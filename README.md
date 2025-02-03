# Flowbyte Interpreter (fbi)

FBI est un interpréteur de fichier binaire FlowByte, écrit en C. Les fichiers binaires peuvent être générés via le compilateur [FBC](https://github.com/ruffaultravenelg/fbc).

## Compilation du code source
Copier le dépôt :
```sh
git clone https://github.com/ruffaultravenelg/fbi.git
cd fbi
```

Compiler le projet en utilisant le fichier `make.bat` ou `Makefile` selon votre système d'exploitation.
```sh
make
```

## Utilisation
Pour exécuter un fichier binaire :
```sh
fb [filename]
```

Pour voir les instructions contenues dans un fichier :
```sh
fb [filename] --read
```

## Contenu du fichier binaire
Un fichier FlowByte contient principalement une liste de fonction, fonction qui contiennes des instructions.

**Fichier**
| Nom d'usage        | Type                     |
| ------------------ | ------------------------ |
| `SIGNATURE`        | `"KITO_FLOWBYTE_BINARY"` |
| `manifest_version` | `f64`                    |
| `entry_point`      | `u32`                    |
| `function_count`   | `u32`                    |
| `[functions...]`   | `fun[]`                  |

**Une fonction *(fun)***
| Nom d'usage         | Type     |
| ------------------- | -------- |
| `name_length`       | `u32`    |
| `[chars...]`        | `char[]` |
| `public`            | `bool`   |
| `arg_count`         | `u32`    |
| `instruction_count` | `u32`    |
| `[instructions...]` | `inst[]` |

**Une instruction *(inst)***
| Nom d'usage | Type  |
| ----------- | ----- |
| `INST_TYPE` | `i32` |
| `ARG1`      | `arg` |
| `ARG2`      | `arg` |


**Un argument *(arg)***
| Nom d'usage | Type  |
| ----------- | ----- |
| `ARG_TYPE`  | `i32` |
| `ARG_VALUE` | `i32` |
