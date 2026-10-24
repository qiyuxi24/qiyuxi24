# 佀佳超 · qiyuxi24

西北工业大学 · 人工智能学院 · 2029 届（本科）

> 把大模型塞进真实系统，并且给它建一套能复现的评测。

做 AI 应用的人很多，能做「这个模型在什么条件下会坏、坏了怎么量出来」的人少。
我这两年主要在练后半句：Agent 的护栏、检索的取舍、评测集怎么设计。

## 在做的

### [AI Tutor](https://github.com/qiyuxi24/AI-tutor) · 知识图谱导学的学习 Agent
不是「套壳 ChatGPT」，是一条可测的 Agent 主链路：意图 → 工具选择 → 参数提取 → 结果利用 → 协议合规。

- 后端 33.6k 行 Python，**413 条用例全绿**（25s 跑完、零网络依赖）
- **四层真实 API 评测**（L1 工具选择 / L2 参数提取 / L3 结果利用 / L4 协议合规）：MiniMax-M3 上 **7/7**
- 检索侧：BM25 门控 + RRF 融合的混合召回；图谱侧：Kahn 拓扑排序 + 断环（`core/kg/paths.py`）
- Agent Loop 四道止血：轮数上限、单工具超时、无工具时丢弃幻觉 tool_call、强制收尾
- 评测数据与 trace 全落盘，A/B 可复跑（`backend/eval_data/`）

### [Votek](https://github.com/qiyuxi24/Agent) · 本地优先的 AI 工作台（桌面）
Tauri v2 + React 19 + Rust。集成 code-server、MCP stdio、向量检索；Tauri 打包体积比 Electron 小一个量级。
写它的时候顺手修了 13 处技术债，其中两处是 Windows 下真会崩的：`#[cfg(windows)]` 守卫缺失、多字节 UTF-8 截断开 panic。

### [bili-archive](https://github.com/qiyuxi24/bili-archive) · 视频本地归档
双路抓流回退 + 双引擎转写。拆分成两层而不是一个 `main.py`，判定标准只有一条：
**能不能脱离队列和配置单独使用**。

## 也在意的事

- **写文档就是写设计**：每个模块的 README 说清「为什么这么拆、失败模式在哪」，而不是罗列技术栈。
- **不假装懂**：`TECH_DEBT.md` / `TODO.md` 都是公开的，欠的债写在那。
- 中文写作为辅：[writing-essay-skills](https://github.com/qiyuxi24/writing-essay-skills)（兼容 SKILL.md 规范的写作 Skill 套件）

## 技术栈

只有真的写过才写：

`Python` `C / C++` `Rust` `Vue3` `React` · `FastAPI` `Tauri` `Docker` `SQLite` · `PyTorch` `NumPy`（手写反向传播对拍）

## 现在

AI Tutor 三赛同投（中国国际大学生创新大赛 / AI+教育创新应用 / AIC 全球校园算法精英大赛），
11 月省赛与决赛。下一步是把评测集从「我的数据集」扩成「别人也能跑的数据集」。

## 联系

- 主页：https://qiyuxi24.github.io/
- 写给我：主页底部有联系方式
