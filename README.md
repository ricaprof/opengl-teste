# 🧊 Editor 3D OpenGL

![C++](https://img.shields.io/badge/C%2B%2B-17-blue.svg)
![OpenGL](https://img.shields.io/badge/OpenGL-Classic-green.svg)
![FreeGLUT](https://img.shields.io/badge/FreeGLUT-Framework-orange.svg)
![Linux](https://img.shields.io/badge/Platform-Linux-lightgrey.svg)

Um editor 3D interativo desenvolvido em **C++**, utilizando **OpenGL** e **FreeGLUT**, com foco educacional e demonstração prática de conceitos fundamentais de **Computação Gráfica**, **transformações geométricas**, **renderização 3D**, **controle de câmera** e **persistência de cenas**.

O projeto permite criar e manipular objetos tridimensionais em tempo real, alterar propriedades visuais e navegar livremente pelo ambiente virtual.

---

## 📸 Visão Geral

O Editor 3D oferece um ambiente simples e intuitivo para:

* Criar objetos geométricos básicos.
* Mover, rotacionar e escalar objetos.
* Alterar cores individualmente.
* Navegar pela cena através de uma câmera orbital.
* Alternar modos de visualização.
* Salvar e restaurar cenas.

---

## ✨ Funcionalidades

### 🎲 Criação de Objetos

* Inserção dinâmica de cubos.
* Inserção dinâmica de esferas.
* Seleção de objetos por clique (picking).

### 🔄 Transformações

* Movimentação nos eixos X, Y e Z.
* Rotação livre de objetos.
* Controle de escala.
* Reset de posição.

### 🎨 Personalização

* Ajuste independente dos canais:

  * Vermelho (R)
  * Verde (G)
  * Azul (B)

### 📷 Controle de Câmera

* Rotação orbital em torno da cena.
* Visualização em perspectiva.
* Visualização ortográfica.

### 🌍 Ambiente

* Sistema de iluminação OpenGL.
* Exibição de eixos cartesianos.
* Exibição de grid de referência.

### 💾 Persistência

* Salvamento da cena em arquivo local.
* Carregamento de cenas previamente salvas.

---

## ⚙️ Requisitos

### Sistema Operacional

* Linux (Ubuntu, Debian, Arch Linux e derivados)

### Dependências

* GCC / G++
* OpenGL
* GLU
* FreeGLUT

---

## 📦 Instalação das Dependências

### Ubuntu / Debian

```bash
sudo apt update
sudo apt install build-essential libgl1-mesa-dev libglu1-mesa-dev freeglut3-dev
```

### Arch Linux

```bash
sudo pacman -S base-devel freeglut mesa glu
```

### Fedora

```bash
sudo dnf install gcc-c++ freeglut-devel mesa-libGL-devel mesa-libGLU-devel
```

---

## 🚀 Compilação

No diretório do projeto execute:

```bash
g++ -o editor3d \
    main.cpp \
    object.cpp \
    scene.cpp \
    camera.cpp \
    -lGL -lGLU -lglut
```

---

## ▶️ Execução

Após a compilação:

```bash
./editor3d
```

---

# 🎮 Controles

## 🖱️ Mouse

| Ação                       | Resultado                      |
| -------------------------- | ------------------------------ |
| Clique esquerdo            | Seleciona um objeto            |
| Clique esquerdo + arrastar | Rotaciona a câmera             |
| Clique direito + arrastar  | Rotaciona o objeto selecionado |
| Clique direito             | Abre menu de contexto          |

---

## ⌨️ Teclado

### Criação e Sistema

| Tecla | Ação               |
| ----- | ------------------ |
| C     | Criar Cubo         |
| S     | Criar Esfera       |
| O     | Carregar cena      |
| R     | Salvar cena        |
| Esc   | Encerrar aplicação |

---

### Movimentação

| Tecla     | Ação                       |
| --------- | -------------------------- |
| X         | Travar movimento no eixo X |
| Y         | Travar movimento no eixo Y |
| Z         | Travar movimento no eixo Z |
| 0         | Movimento livre            |
| + / =     | Movimento positivo         |
| - / _     | Movimento negativo         |
| ↑ ↓ ← →   | Movimentação no plano XY   |
| Page Up   | Movimento positivo em Z    |
| Page Down | Movimento negativo em Z    |
| Home      | Resetar posição            |

---

### Controle de Cores

| Tecla | Ação                         |
| ----- | ---------------------------- |
| R / E | Aumentar / Diminuir vermelho |
| G / F | Aumentar / Diminuir verde    |
| B / V | Aumentar / Diminuir azul     |

---

### Visualização

| Tecla | Ação                             |
| ----- | -------------------------------- |
| L     | Ativar/Desativar iluminação      |
| P     | Alternar perspectiva/ortográfica |
| A     | Mostrar/Ocultar eixos            |
| G     | Mostrar/Ocultar grid             |

---

## 💾 Arquivo de Cena

As cenas são armazenadas em:

```text
scene.txt
```

São persistidas informações como:

* Tipo do objeto
* Posição
* Escala
* Cor
* Estado geral da cena

---

## 🗂️ Estrutura do Projeto

```text
.
├── main.cpp
├── camera.h
├── camera.cpp
├── object.h
├── object.cpp
├── scene.h
├── scene.cpp
├── scene.txt
└── README.md
```

### main.cpp

Responsável por:

* Inicialização do OpenGL
* Loop principal do GLUT
* Tratamento de eventos
* Menus
* Renderização da HUD

### camera.*

Implementa:

* Câmera orbital
* Matrizes de visualização
* Controle de navegação

### object.*

Define:

* Estrutura dos objetos 3D
* Renderização
* Transformações geométricas

### scene.*

Gerencia:

* Coleção de objetos
* Picking (seleção)
* Salvamento e carregamento de cenas

---

## 🏗️ Tecnologias Utilizadas

* C++17
* OpenGL (Fixed Pipeline)
* GLU
* FreeGLUT

---

## 🎯 Objetivos Educacionais

Este projeto foi desenvolvido para praticar:

* Programação orientada a objetos em C++
* Computação gráfica
* Transformações 3D
* Sistemas de coordenadas
* Renderização em OpenGL clássico
* Interação por mouse e teclado
* Persistência de dados em arquivos

---

## 📄 Licença

Este projeto pode ser utilizado livremente para fins acadêmicos, educacionais e de estudo.

---

## 👨‍💻 Autor

Desenvolvido como projeto de estudo e prática em Computação Gráfica utilizando OpenGL e C++.
