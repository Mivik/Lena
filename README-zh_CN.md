<h1 align="center">Lena</h1>
<h4 align="center">Cena client for Linux.</h4>
<p>
  <img alt="Version" src="https://img.shields.io/badge/version-1.0.0-blue.svg?cacheSeconds=2592000" />
  <a href="./LICENSE.md" target="_blank">
    <img alt="License: GPL-3.0" src="https://img.shields.io/badge/License-GPL--3.0-yellow.svg" />
  </a>
</p>

## Lena 是什么？

Lena 是用于 Linux 的 Cena 客户端，弥补了 Cena 官方没有中文客户端的缺陷。

## 如何下载 Lena？

请查看 [Release 界面](https://github.com/Mivik/Lena/releases)。

## 如何编译 Lena？

首先，安装所有依赖：

```shell
sudo apt install cmake pkg-config libappindicator3-dev libgtkmm-3.0-dev
```

然后创建一个临时文件夹，并在其中构建 Lena：

```shell
mkdir build && cd build
cmake ..
make
```

## 截图

![主界面](https://s1.ax1x.com/2020/07/17/U6efmj.png)

## 使用中出现问题？

欢迎在 [issue 界面](https://github.com/Mivik/Lena/issues/new) 反馈！