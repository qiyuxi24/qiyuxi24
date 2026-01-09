# NOJ_Solution

> 基于 C++ 的 4K 音游模拟器（仿 Malody）

---

## 零 · 简介

这是西北工业大学 C 程序设计的大作业，基于 C++ 模仿 malody 实现一个简单的 4k 音游。

---

## 一 · 流程统计

- **开场动画**
- **本地登录（选做）**
- **选曲菜单**
    - 谱面信息
    - 背景音乐（随机播放）
    - 排序、搜索、收藏夹
    - 设置
    - ...
- **难度选择**
    - 音乐预览
- **加载界面**（按需制作）
- **游玩**
    - 如果没有键位设置需要设置键位
    - 背景静态图片
    - 进度条
    - 音乐、画面、判定、连击数
    - 暂停 → 设置
- **结算**
    - 统计
    - 准确率 ACC
    - 排名（和本地登录一同实现）
    - 截图

---

## 二 · 成品结构

    NOJ_Solution/
    ├── beatmap/                 # 谱面资源目录
    │   └── 曲目名称/
    │       ├── background.jpg   # 背景图片
    │       ├── music.ogg        # 背景音乐
    │       └── chart.mc         # 谱面文件
    ├── users/                   # 用户数据
    │   └── username/
    │       └── user.data        # 账号密码成绩
    ├── logs/                    # 日志文件夹
    ├── screenshot/              # 成绩截图
    ├── config.data              # 全局配置
    ├── *.dl
    ├── *.dll
    └── play.exe                 # 游戏主程序

---

## 三 · 开发模块

### `namespace FileManager`

文件读写模块

- `Beatmap` // 读谱面文件（包括图片音频）
- `Config` // 读写配置文件
- `User` // 读写用户信息、成绩
- `Screenshot` // 读写成绩截图
- `Logger` // 日志记录

### `namespace Game`

游戏核心逻辑模块

- `Audio` // 音频处理
- `Renderer` // 图形渲染
- `Input` // 输入处理
- `Gameplay` // 游戏玩法
    - `Note` // 音符
    - `Judge` // 判定逻辑，计算判定结果
    - `Score` // 分数计算，计算准确率、连击数等

### `UI`

用户界面模块

- `UIManager` // UI 管理器，负责管理 UI 元素
- `Screen` // 屏幕，如主菜单、游戏界面、结算界面
- `Menu` // 菜单，如选曲菜单、设置菜单

### `namespace Tools`

工具模块

- `Timer` // 计时器，用于精确的时间控制
- `JsonParser` // JSON 解析器，用于解析谱面和配置文件

---

## 四 · 外部资源

- 图形库：`SFML库`
- Json库：`nlohmann_json库`
- 铺面文件：`malody内下载`