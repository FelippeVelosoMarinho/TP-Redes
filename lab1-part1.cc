/*
 * SPDX-License-Identifier: GPL-2.0-only
 */

 #include "ns3/applications-module.h"
 #include "ns3/core-module.h"
 #include "ns3/internet-module.h"
 #include "ns3/network-module.h"
 #include "ns3/point-to-point-module.h"

 #include <iostream>
 #include <sstream>
 
 // Default Network Topology
 //
 //       10.1.1.0
 // n0 -------------- n1
 //    point-to-point
 //
 
 using namespace ns3;
 
 NS_LOG_COMPONENT_DEFINE("FirstScriptExample");
 
 int
 main(int argc, char* argv[])
 {
     int nClients = 1;
     int nPackets = 1;
     CommandLine cmd(__FILE__);
     // declaracao de --nClients e --nPackets
     cmd.AddValue("nClients", "Numero de clientes (1-5)", nClients);
     cmd.AddValue("nPackets", "Numero de pacotes por cliente (1-5)", nPackets);
     cmd.Parse(argc, argv);

     if (nClients < 1 || nClients > 5 || nPackets < 1 || nPackets > 5) {
        std::cerr << "Erro: nClients e nPackets devem ser valores entre 1 e 5." << std::endl;
        return 1;
    }
 
     Time::SetResolution(Time::NS);
     LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
     LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
 
     NodeContainer server;
     server.Create(1); // n0

     NodeContainer clients;
     clients.Create(nClients); // nós n1, n2 ...
 
    // Enlaces ponto a ponto 
     PointToPointHelper pointToPoint;
     pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));
     pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));
 
     InternetStackHelper stack;
     stack.SetIpv6StackInstall(false);
     stack.Install(server);
     stack.Install(clients);

 
     Ipv4AddressHelper address;
     Ipv4InterfaceContainer interfaces;

     for (uint32_t i = 0; i < (uint32_t)nClients; ++i) {
        // Liga o servidor (n0) a cada cliente (n1, n2, ...) individualmente
        NetDeviceContainer link = pointToPoint.Install(server.Get(0), clients.Get(i));
    
        // Configura a sub-rede dinamicamente: 10.1.1.0, 10.1.2.0, ...
        std::ostringstream subnet;
        subnet << "10.1." << (i + 1) << ".0";
        address.SetBase(subnet.str().c_str(), "255.255.255.0");
    
        Ipv4InterfaceContainer ifaces = address.Assign(link);
        
        // Guarda as interfaces do primeiro link; GetAddress(0) = servidor
        if (i == 0) {
            interfaces = ifaces;
        }
    }

     Ipv4GlobalRoutingHelper::PopulateRoutingTables();
 
     UdpEchoServerHelper echoServer(9);
 
     ApplicationContainer serverApps = echoServer.Install(server.Get(0));
     serverApps.Start(Seconds(0.0));
     serverApps.Stop(Seconds(20.0));
 
     UdpEchoClientHelper echoClient(interfaces.GetAddress(0), 9);
     echoClient.SetAttribute("MaxPackets", UintegerValue(nPackets));
     echoClient.SetAttribute("Interval", TimeValue(Seconds(1)));
     echoClient.SetAttribute("PacketSize", UintegerValue(1024));

     Ptr<UniformRandomVariable> uv = CreateObject<UniformRandomVariable>();
     uv->SetAttribute("Min", DoubleValue(2.0));
     uv->SetAttribute("Max", DoubleValue(7.0));
     for (uint32_t i = 0; i < (uint32_t)nClients; ++i) {
        ApplicationContainer clientApp = echoClient.Install(clients.Get(i));
        
        // Início sorteado entre 2.0 e 7.0 segundos
        Time startTime = Seconds(uv->GetValue());
        clientApp.Start(startTime);
        clientApp.Stop(Seconds(20.0));
    }
 
     Simulator::Run();
     Simulator::Destroy();
     return 0;
 }
 