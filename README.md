<h1 align="center">Lena</h1>
<h4 align="center">Cena client for Linux.</h4>
<p>
  <img alt="Version" src="https://img.shields.io/badge/version-1.0.0-blue.svg?cacheSeconds=2592000" />
  <a href="./LICENSE.md" target="_blank">
    <img alt="License: GPL-3.0" src="https://img.shields.io/badge/License-GPL--3.0-yellow.svg" />
  </a>
</p>

[中文](./README-zh_CN.md)

## What is Lena?

Lena is a Cena client created for Linux since Cena's official client does not support Linux.

## How can I get Lena?

Check out the [release page](https://github.com/Mivik/Lena/releases).

## How can I compile Lena?

First, install all dependencies:

```shell
sudo apt install cmake pkg-config libappindicator3-dev libgtkmm-3.0-dev
```

Then create a temporary directory and then build Lena in it:

```shell
mkdir build && cd build
cmake ..
make
```

## Screenshots

![Main Window](https://s1.ax1x.com/2020/07/17/U6efmj.png)

## Encountered problems when using?

Please create a issue [here](https://github.com/Mivik/Lena/issues/new).