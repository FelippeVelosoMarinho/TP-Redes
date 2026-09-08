# TP-Redes — uso do ns-3 neste repositório

Instalação do ns-3 usada: `/home/felippe/Documentos/ns-3.48`

## Links simbólicos

Para editar os arquivos neste repo e compilar/rodar com o `./ns3` da instalação externa, foram criados links simbólicos em duas pontas.

### 1. Script `ns3` no repositório

Permite chamar `./ns3` a partir da raiz deste repo. O script resolve o caminho real da instalação via `realpath`, então funciona mesmo sendo um link.

```bash
ln -sfn /home/felippe/Documentos/ns-3.48/ns3 \
  /home/felippe/Documentos/Github/UFMG/TP-Redes/ns3
```

### 2. Arquivos `.cc` no `scratch/` do ns-3

O ns-3 só compila programas em `scratch/`. Os links apontam para os fontes deste repo:

```bash
REPO=/home/felippe/Documentos/Github/UFMG/TP-Redes
NS3=/home/felippe/Documentos/ns-3.48

# Labs
ln -sfn "$REPO/lab1-part1.cc" "$NS3/scratch/lab1-part1.cc"
ln -sfn "$REPO/lab1-part2.cc" "$NS3/scratch/lab1-part2.cc"
ln -sfn "$REPO/lab1-part3.cc" "$NS3/scratch/lab1-part3.cc"

# Exemplos de referência (refs/)
ln -sfn "$REPO/refs/first.cc"  "$NS3/scratch/first.cc"
ln -sfn "$REPO/refs/second.cc" "$NS3/scratch/second.cc"
ln -sfn "$REPO/refs/third.cc"  "$NS3/scratch/third.cc"
```

### Resultado esperado

| Local | Link | Destino |
|-------|------|---------|
| `TP-Redes/ns3` | → | `ns-3.48/ns3` |
| `ns-3.48/scratch/lab1-part1.cc` | → | `TP-Redes/lab1-part1.cc` |
| `ns-3.48/scratch/lab1-part2.cc` | → | `TP-Redes/lab1-part2.cc` |
| `ns-3.48/scratch/lab1-part3.cc` | → | `TP-Redes/lab1-part3.cc` |
| `ns-3.48/scratch/first.cc` | → | `TP-Redes/refs/first.cc` |
| `ns-3.48/scratch/second.cc` | → | `TP-Redes/refs/second.cc` |
| `ns-3.48/scratch/third.cc` | → | `TP-Redes/refs/third.cc` |

## Compilar e rodar

Na raiz deste repositório:

```bash
./ns3 configure --enable-examples
./ns3 build

./ns3 run lab1-part1
./ns3 run lab1-part2
./ns3 run lab1-part3
```

## Recriar os links

Se os caminhos mudarem ou os links forem removidos, rode de novo os comandos `ln -sfn` das seções acima (`-f` sobrescreve links existentes).
