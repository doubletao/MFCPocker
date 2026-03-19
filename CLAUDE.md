# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## 项目概述

这是一个基于 MFC 的斗地主扑克游戏（Chinese Poker / Dou Di Zhu），Windows 桌面应用程序。

**技术栈：** C++ + MFC + MSVC，目标平台 Win32 (x86)

## 常用构建命令

### 使用 Visual Studio IDE
1. 打开 `Poker.sln` 解决方案文件
2. 选择配置（Debug 或 Release）
3. F7 生成解决方案

### 使用命令行 MSBuild
```bash
# 编译 Debug 版本
msbuild Poker.sln /p:Configuration=Debug /p:Platform=Win32

# 编译 Release 版本
msbuild Poker.sln /p:Configuration=Release /p:Platform=Win32
```

**输出位置：** `Debug/Poker.exe` 或 `Release/Poker.exe`

## 代码架构

### 核心类设计

| 类名 | 文件 | 职责 |
|------|------|------|
| `CPokerApp` | Poker.cpp/h | 应用程序入口，管理生命周期 |
| `CPokerDlg` | PokerDlg.cpp/h | 主对话框，处理用户交互和界面绘制 |
| `DataCenter` | DataCenter.cpp/h | 数据中心（单例），管理全局游戏数据 |
| `Judge` | Judge.cpp/h | 游戏裁判（单例），规则判断和流程控制 |
| `Player` | Player.cpp/h | 玩家类，管理单个玩家的牌和状态 |
| `CardGroup` | CardGroup.cpp/h | 牌组类，管理牌组合和牌型判断 |
| `CardPile` | CardPile.cpp/h | 牌堆容器类 |
| `PokerCard` | PokerCard.cpp/h | 单张扑克牌，存储花色和点数 |
| `RobotPilot` | RobotPilot.cpp/h | AI 机器人，控制电脑玩家自动出牌 |
| `GDIButton` | GDIButton.cpp/h | 自定义 GDI 绘制按钮 |

### 接口设计
- `ICard` - 卡牌接口
- `IClickable` - 可点击接口
- `IDrawable` - 可绘制接口
- `ISortable` - 可排序接口

### 游戏流程
1. **初始化** → 2. **发牌** → 3. **叫地主/抢地主** → 4. **轮流出牌** → 5. **胜负判断**

### 设计模式
- **单例模式**：DataCenter 和 Judge 全局唯一实例
- **面向接口编程**：抽离通用行为接口
- **RAII**：资源自动管理

## 文件结构

```
MFCPoker/
├── *.cpp/h          # 核心源代码
├── stdafx.cpp/h     # 预编译头
├── Resource.h       # 资源ID定义
├── Poker.rc         # 资源脚本
├── res/             # 图标等资源文件
├── Poker.sln        # Visual Studio 解决方案
└── Poker.vcxproj    # Visual C++ 项目文件
```

## 开发须知

1. 这是 MFC 动态链接库版本项目，编译需要 MFC 库支持
2. 需要 Visual Studio 安装 C++ MFC 组件
3. 目标平台是 Win32 (x86)，在 x64 系统上需要对应工具链支持
4. 使用预编译头 (stdafx.h) 加快编译
