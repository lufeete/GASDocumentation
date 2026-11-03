# GASDocumentation（中文）

> 译文说明：本文件根据同目录的 [README.md](README.md) 生成，采用中文讲解并保留 float、GA、GE、ASC、Attribute、GameplayAbility 等常用英文术语。正文中的“我”指原作者。译文经机器翻译及术语整理，技术细节可对照英文原文与插件源码；代码示例、API 名称和版本范围沿用原文。

本文通过一个简单的多人游戏示例项目，介绍我对 Unreal Engine 5 的 GameplayAbilitySystem（GAS）插件的理解。这不是官方文档，本项目及我本人均不隶属于 Epic Games。我不保证这些信息完全准确。

本文旨在解释 GAS 的主要概念和类，并结合我的使用经验提供补充说明。GAS 社区中积累了许多依靠交流传承的实践经验，我希望在这里分享自己掌握的内容。

示例项目和文档适用于**虚幻引擎 5.3** (UE5)。本文档有针对旧版本虚幻引擎的分支，但它们不再受支持，并且可能存在错误或过时的信息。请使用与您的引擎版本匹配的分支。

[GASShooter](https://github.com/tranek/GASShooter) 是一个姐妹示例项目，展示了使用 GAS 实现多人 FPS/TPS 的先进技术。

最好的文档永远是插件源代码。

<a name="table-of-contents"></a>
## 目录

> 1. [GameplayAbilitySystem 插件简介](#intro)
> 1. [示例项目](#sp)
> 1. [配置使用 GAS 的项目](#setup)
> 1. [概念](#concepts)  
>    4.1 [Ability System Component（ASC）](#concepts-asc)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.1.1 [Replication Mode（复制模式）](#concepts-asc-rm)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.1.2 [配置与初始化](#concepts-asc-setup)  
>    4.2 [Gameplay Tags](#concepts-gt)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.2.1 [响应 Gameplay Tag 的变化](#concepts-gt-change)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.2.2 [从插件的 .ini 文件加载 Gameplay Tags](#concepts-gt-loadfromplugin)  
>    4.3 [Attributes](#concepts-a)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.3.1 [Attribute 的定义](#concepts-a-definition)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.3.2 [BaseValue 与 CurrentValue](#concepts-a-value)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.3.3 [Meta Attributes](#concepts-a-meta)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.3.4 [响应 Attribute 的变化](#concepts-a-changes)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.3.5 [派生 Attributes](#concepts-a-derived)  
>    4.4 [AttributeSet](#concepts-as)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.4.1 [AttributeSet 的定义](#concepts-as-definition)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.4.2 [AttributeSet 的设计](#concepts-as-design)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.4.2.1 [具有独立 Attributes 的子组件](#concepts-as-design-subcomponents)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.4.2.2 [在运行时添加和移除 AttributeSet](#concepts-as-design-addremoveruntime)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.4.2.3 [物品的 Attributes（武器弹药）](#concepts-as-design-itemattributes)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.4.2.3.1 [在物品上使用普通 float](#concepts-as-design-itemattributes-plainfloats)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.4.2.3.2 [在物品上使用 `AttributeSet`](#concepts-as-design-itemattributes-attributeset)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.4.2.3.3 [在物品上使用 `ASC`](#concepts-as-design-itemattributes-asc)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.4.3 [定义 Attributes](#concepts-as-attributes)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.4.4 [初始化 Attributes](#concepts-as-init)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.4.5 [PreAttributeChange()](#concepts-as-preattributechange)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.4.6 [PostGameplayEffectExecute()](#concepts-as-postgameplayeffectexecute)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.4.7 [OnAttributeAggregatorCreated()](#concepts-as-onattributeaggregatorcreated)  
>    4.5 [Gameplay Effects（GE）](#concepts-ge)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.1 [Gameplay Effect 的定义](#concepts-ge-definition)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.2 [应用 Gameplay Effects](#concepts-ge-applying)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.3 [移除 Gameplay Effects](#concepts-ga-removing)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.4 [Gameplay Effect 的 Modifiers](#concepts-ge-mods)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.4.1 [Multiply 和 Divide Modifiers](#concepts-ge-mods-multiplydivide)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.4.2 [Modifier 上的 Gameplay Tags](#concepts-ge-mods-gameplaytags)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.5 [Gameplay Effect 的叠加](#concepts-ge-stacking)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.6 [授予的 Abilities](#concepts-ge-ga)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.7 [Gameplay Effect 的 Tags](#concepts-ge-tags)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.8 [免疫](#concepts-ge-immunity)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.9 [Gameplay Effect Spec](#concepts-ge-spec)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.9.1 [SetByCallers](#concepts-ge-spec-setbycaller)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.10 [Gameplay Effect Context](#concepts-ge-context)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.11 [Modifier Magnitude Calculation（MMC）](#concepts-ge-mmc)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.12 [Gameplay Effect Execution Calculation（ExecCalc）](#concepts-ge-ec)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.12.1 [向 Execution Calculation 传递数据](#concepts-ge-ec-senddata)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.12.1.1 [SetByCaller](#concepts-ge-ec-senddata-setbycaller)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.12.1.2 [使用 Attribute 作为数据来源的 Calculation Modifier](#concepts-ge-ec-senddata-backingdataattribute)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.12.1.3 [使用临时变量作为数据来源的 Calculation Modifier](#concepts-ge-ec-senddata-backingdatatempvariable)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.12.1.4 [Gameplay Effect Context](#concepts-ge-ec-senddata-effectcontext)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.13 [自定义应用条件（Custom Application Requirement）](#concepts-ge-car)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.14 [Cost Gameplay Effect](#concepts-ge-cost)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.15 [Cooldown Gameplay Effect](#concepts-ge-cooldown)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.15.1 [获取 Cooldown Gameplay Effect 的剩余时间](#concepts-ge-cooldown-tr)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.15.2 [监听冷却开始与结束](#concepts-ge-cooldown-listen)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.15.3 [预测冷却](#concepts-ge-cooldown-prediction)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.16 [修改生效中的 Gameplay Effect 的持续时间](#concepts-ge-duration)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.17 [在运行时动态创建 Gameplay Effects](#concepts-ge-dynamic)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.5.18 [Gameplay Effect Containers](#concepts-ge-containers)  
>    4.6 [Gameplay Abilities（GA）](#concepts-ga)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.1 [Gameplay Ability 的定义](#concepts-ga-definition)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.1.1 [Replication Policy（复制策略）](#concepts-ga-definition-reppolicy)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.1.2 [Server Respects Remote Ability Cancellation](#concepts-ga-definition-remotecancel)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.1.3 [Replicate Input Directly](#concepts-ga-definition-repinputdirectly)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.2 [将输入绑定到 ASC](#concepts-ga-input)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.2.1 [绑定输入但不激活 Ability](#concepts-ga-input-noactivate)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.3 [授予 Abilities](#concepts-ga-granting)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.4 [激活 Abilities](#concepts-ga-activating)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.4.1 [被动 Abilities](#concepts-ga-activating-passive)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.4.2 [激活失败的 Tags](#concepts-ga-activating-failedtags)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.5 [取消 Abilities](#concepts-ga-cancelabilities)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.6 [获取激活中的 Abilities](#concepts-ga-definition-activeability)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.7 [Instancing Policy（实例化策略）](#concepts-ga-instancing)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.8 [Net Execution Policy（网络执行策略）](#concepts-ga-net)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.9 [Ability Tags](#concepts-ga-tags)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.10 [Gameplay Ability Spec](#concepts-ga-spec)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.11 [向 Ability 传递数据](#concepts-ga-data)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.12 [Ability 的消耗与冷却](#concepts-ga-commit)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.13 [提升 Ability 的等级](#concepts-ga-leveling)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.14 [Ability Sets](#concepts-ga-sets)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.15 [Ability 批处理](#concepts-ga-batching)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.6.16 [Net Security Policy（网络安全策略）](#concepts-ga-netsecuritypolicy)  
>    4.7 [Ability Tasks](#concepts-at)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.7.1 [Ability Task 的定义](#concepts-at-definition)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.7.2 [自定义 Ability Tasks](#concepts-at-definition)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.7.3 [使用 Ability Tasks](#concepts-at-using)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.7.4 [Root Motion Source Ability Tasks](#concepts-at-rms)  
>    4.8 [Gameplay Cues（GC）](#concepts-gc)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.8.1 [Gameplay Cue 的定义](#concepts-gc-definition)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.8.2 [触发 Gameplay Cues](#concepts-gc-trigger)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.8.3 [本地 Gameplay Cues](#concepts-gc-local)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.8.4 [Gameplay Cue 的参数](#concepts-gc-parameters)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.8.5 [Gameplay Cue Manager](#concepts-gc-manager)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.8.6 [阻止触发 Gameplay Cues](#concepts-gc-prevention)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.8.7 [Gameplay Cue 批处理](#concepts-gc-batching)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.8.7.1 [手动调用 RPC](#concepts-gc-batching-manualrpc)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.8.7.2 [一个 GE 上的多个 GC](#concepts-gc-batching-gcsonge)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.8.8 [Gameplay Cue 的事件](#concepts-gc-events)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.8.9 [Gameplay Cue 的可靠性](#concepts-gc-reliability)  
>    4.9 [Ability System Globals](#concepts-asg)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.9.1 [InitGlobalData()](#concepts-asg-initglobaldata)  
>    4.10 [预测](#concepts-p)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.10.1 [Prediction Key](#concepts-p-key)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.10.2 [在 Ability 中创建新的预测窗口](#concepts-p-windows)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.10.3 [预测生成 Actors](#concepts-p-spawn)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.10.4 [GAS 预测机制的未来](#concepts-p-future)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.10.5 [Network Prediction 插件](#concepts-p-npp)  
>    4.11 [目标选择](#concepts-targeting)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.11.1 [Target Data](#concepts-targeting-data)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.11.2 [Target Actors](#concepts-targeting-actors)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.11.3 [Target Data Filters](#concepts-target-data-filters)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.11.4 [Gameplay Ability World Reticles](#concepts-targeting-reticles)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;4.11.5 [Gameplay Effect Container 的目标选择](#concepts-targeting-containers)  
> 1. [常见 Ability 和 Effect 的实现](#cae)  
>    5.1 [眩晕](#cae-stun)  
>    5.2 [冲刺](#cae-sprint)  
>    5.3 [开镜瞄准（ADS）](#cae-ads)  
>    5.4 [生命偷取](#cae-ls)  
>    5.5 [在客户端和服务器生成随机数](#cae-random)  
>    5.6 [暴击](#cae-crit)  
>    5.7 [不叠加 Gameplay Effects，仅使数值最大的效果作用于目标](#cae-nonstackingge)  
>    5.8 [在游戏暂停时生成 Target Data](#cae-paused)  
>    5.9 [单键交互系统](#cae-onebuttoninteractionsystem)  
> 1. [调试 GAS](#debugging)  
>    6.1 [showdebug abilitysystem](#debugging-sd)  
>    6.2 [Gameplay Debugger](#debugging-gd)  
>    6.3 [GAS 日志](#debugging-log)  
> 1. [优化](#optimizations)  
>    7.1 [Ability 批处理](#optimizations-abilitybatching)  
>    7.2 [Gameplay Cue 批处理](#optimizations-gameplaycuebatching)  
>    7.3 [AbilitySystemComponent 的 Replication Mode](#optimizations-ascreplicationmode)  
>    7.4 [Attribute 代理复制](#optimizations-attributeproxyreplication)  
>    7.5 [ASC 延迟加载](#optimizations-asclazyloading)  
> 1. [改善开发体验的建议](#qol)  
>    8.1 [Gameplay Effect Containers](#qol-gameplayeffectcontainers)  
>    8.2 [通过蓝图 AsyncTask 绑定 ASC 的 Delegates](#qol-asynctasksascdelegates)  
> 1. [故障排查](#troubleshooting)  
>    9.1 [`LogAbilitySystem: Warning: Can't activate LocalOnly or LocalPredicted ability %s when not local!`](#troubleshooting-notlocal)  
>    9.2 [`ScriptStructCache` 错误](#troubleshooting-scriptstructcache)  
>    9.3 [Animation Montage 未复制到客户端](#troubleshooting-replicatinganimmontages)  
>    9.4 [复制蓝图 Actor 导致 AttributeSet 变为 nullptr](#troubleshooting-duplicatingblueprintactors)  
>    9.5 [无法解析外部符号 UEPushModelPrivate::MarkPropertyDirty(int,int)](#troubleshooting-unresolvedexternalsymbolmarkpropertydirty)  
>    9.6 [Enum 名称现在以路径名称表示](#troubleshooting-enumnamesarenowpathnames)  
> 1. [GAS 常用缩写](#acronyms)
> 1. [其他资源](#resources)  
>    11.1 [与 Epic Games 的 Dave Ratti 问答](#resources-daveratti)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;11.1.1 [社区问题（一）](#resources-daveratti-community1)  
>    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;11.1.2 [社区问题（二）](#resources-daveratti-community2)  
> 1. [GAS 更新记录](#changelog)  
>    * [5.3](#changelog-5.3)  
>    * [5.2](#changelog-5.2)  
>    * [5.1](#changelog-5.1)  
>    * [5.0](#changelog-5.0)  
>    * [4.27](#changelog-4.27)  
>    * [4.26](#changelog-4.26)  
>    * [4.25.1](#changelog-4.25.1)  
>    * [4.25](#changelog-4.25)  
>    * [4.24](#changelog-4.24)
         
<a name="intro"></a>
## 1. GameplayAbilitySystem 插件简介
来自 [官方文档](https://docs.unrealengine.com/en-US/Gameplay/GameplayAbilitySystem/index.html)：
>GameplayAbility系统是一个高度灵活的框架，用于构建您可能在 RPG 或 MOBA 游戏中找到的类型的能力和属性。您可以为游戏中的角色构建要使用的动作或被动能力，可以因这些动作而增强或削弱各种属性的状态效果，实施“冷却”计时器或资源成本来调节这些动作的使用，更改每个级别的能力及其效果的级别，激活粒子或声音效果等等。简而言之，该系统可以帮助您设计、实现和有效地网络游戏内能力，就像跳跃一样简单，或者像任何现代 RPG 或 MOBA 游戏中您最喜欢的角色的能力设置一样复杂。

GameplayAbilitySystem 插件由 Epic Games 开发，附带虚幻引擎。它已经在 Paragon 和 Fortnite 等 AAA 商业游戏中经过了战斗测试。

该插件为单人和多人游戏提供了开箱即用的解决方案：
* 实施基于等级的角色能力或技能，并具有可选的成本和冷却时间（[GameplayAbilities](#concepts-ga)）
* 操纵属于Actor的数字`Attributes`（[Attributes](#concepts-a)）
* 将状态效果应用于Actor ([GameplayEffects](#concepts-ge))
* 将 `GameplayTags` 应用于Actor ([GameplayTags](#concepts-gt))
* 生成视觉或声音效果 ([GameplayCues](#concepts-gc))
* 复制上述所有内容

在多人游戏中，GAS 提供对以下 [客户端预测](#concepts-p) 的支持：
* 能力激活
* 播放Animation Montage
* `Attributes` 的更改
* 应用`GameplayTags`
* 产卵 `GameplayCues`
* 通过连接到 `CharacterMovementComponent` 的 `RootMotionSource` 功能进行运动。

**GAS必须用C++**设置，但`GameplayAbilities`和`GameplayEffects`可以由设计者在蓝图中创建。

目前 GAS 的问题：
* `GameplayEffect` 延迟协调（无法预测能力冷却时间，导致与延迟较低的玩家相比，延迟较高的玩家对于低冷却能力的射速较低）。
* 无法预测 `GameplayEffects` 的删除。然而，我们可以预测添加 `GameplayEffects` 会产生相反的效果，从而有效地消除它们。这并不总是适当或可行的，并且仍然是一个问题。
* 缺乏样板模板、多人游戏示例和文档。希望这对此有所帮助！

**[⬆ 返回顶部](#table-of-contents)**

<a name="sp"></a>
## 2. 示例项目
本文档中包含一个多人第三人称射击游戏示例项目，面向 GameplayAbilitySystem 插件新手但不熟悉虚幻引擎的用户。用户应该了解 UE 中的 C++、蓝图、UMG、复制和其他中级主题。该项目提供了一个示例，说明如何使用 `PlayerState` 类上用于玩家/AI 控制英雄的 `AbilitySystemComponent` (`ASC`) 和 `ASC` 上用于 AI 控制小兵的 `Character` 类来设置基本的第三人称射击多人游戏就绪项目。

目标是保持该项目简单，同时展示 GAS 基础知识并通过注释良好的代码展示一些常见要求的功能。由于其专注于初学者，该项目不显示 [预测射弹](#concepts-p-spawn) 等高级主题。

概念展示：
* `ASC` 上 `PlayerState` 与 `Character`
* 复制 `Attributes`
* 复制Animation Montage
* `GameplayTags`
* 在 `GameplayAbilities` 内部和外部涂抹和去除 `GameplayEffects`
* 应用护甲减轻的伤害来改变角色的健康状况
* `GameplayEffectExecutionCalculations`
* 眩晕效果
* 死亡与重生
* 从服务器上的能力生成Actor（射弹）
* 通过瞄准和冲刺来预测改变本地玩家的速度
* 不断消耗体力去冲刺
* 使用法力来施放技能
* 被动能力
* 堆叠 `GameplayEffects`
* 瞄准Actor
* 在蓝图中创建的 `GameplayAbilities`
* 用 C++ 创建的 `GameplayAbilities`
* 按 `Actor` `GameplayAbilities` 实例化
* 非实例`GameplayAbilities`（跳转）
* 静态`GameplayCues`（FireGun弹丸撞击粒子效果）
* Actor `GameplayCues`（冲刺和眩晕粒子效果）

英雄职业具有以下能力：

| 能力                    | 输入绑定          | 预测  | C++ / 蓝图 | 描述                                                                                                                                                                  |
| -------------------------- | ------------------- | ---------- | --------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 跳                       | 空格键           | 是的        | C++             | 让英雄跳跃。                                                                                                                                                         |
| 枪                        | 鼠标左键   | 不         | C++             | 从英雄的枪中发射射弹。动画已预测，但射弹未预测。                                                                                |
| 瞄准目标            | 鼠标右键  | 是的        | 蓝图       | 按住按钮时，英雄会走得更慢，镜头会放大，以便用枪进行更精确的射击。                                                    |
| 短跑                     | 左移          | 是的        | 蓝图       | 按住按钮时，英雄会跑得更快，消耗体力。                                                                                                         |
| 向前冲刺               | Q                   | 是的        | 蓝图       | 英雄以体力为代价向前冲刺。                                                                                                                              |
| 被动装甲堆栈       | 被动的             | 不         | 蓝图       | 每 4 秒英雄就会获得一层护甲，最多可达 4 层。受到伤害会移除一层护甲。                                                    |
| 流星                     | R                   | 不         | 蓝图       | 玩家瞄准一个位置，向敌人投下一颗流星，造成伤害并击晕他们。目标是可以预测的，而流星的产生则不是。                     |

`GameplayAbilities` 是用 C++ 还是蓝图创建并不重要。这里使用了两者的混合，例如如何用每种语言进行操作。

Minions 不附带任何预定义的 `GameplayAbilities`。红色小兵有更多的生命恢复，而蓝色小兵有更高的起始生命值。

对于 `GameplayAbility` 命名，我使用后缀 `_BP` 来表示 `GameplayAbility's` 逻辑是在蓝图中创建的。缺少后缀意味着逻辑是用 C++ 创建的。

**蓝图资源命名前缀**

| 前缀      | 资产类型          |
| ----------- | ------------------- |
| 遗传算法_         | GameplayAbility     |
| GC_         | GameplayCue         |
| 通用电气_         | GameplayEffect      |

**[⬆ 返回顶部](#table-of-contents)**

<a name="setup"></a>
## 3. 配置使用 GAS 的项目
使用 GAS 设置项目的基本步骤：
1. 在编辑器中启用 GameplayAbilitySystem 插件
1. 编辑 `YourProjectName.Build.cs` 将 `"GameplayAbilities", "GameplayTags", "GameplayTasks"` 添加到您的 `PrivateDependencyModuleNames`
1. 刷新/重新生成 Visual Studio 项目文件
1. 从4.24开始到5.2，必须调用`UAbilitySystemGlobals::Get().InitGlobalData()`才能使用[`TargetData`](#concepts-targeting-data)。示例项目在 `UAssetManager::StartInitialLoading()` 中执行此操作。这在5.3中称为自动启动。有关详细信息，请参阅 [`InitGlobalData()`](#concepts-asg-initglobaldata)。

这就是启用 GAS 所需要做的全部工作。从这里，将 [`ASC`](#concepts-asc) 和 [`AttributeSet`](#concepts-as) 添加到您的 `Character` 或 `PlayerState` 并开始制作 [`GameplayAbilities`](#concepts-ga) 和 [`GameplayEffects`](#concepts-ge)！

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts"></a>
## 4. GAS 概念

#### 本节目录

> 4.1 [Ability System Component（ASC）](#concepts-asc)  
> 4.2 [Gameplay Tags](#concepts-gt)  
> 4.3 [Attributes](#concepts-a)  
> 4.4 [AttributeSet](#concepts-as)  
> 4.5 [Gameplay Effects（GE）](#concepts-ge)  
> 4.6 [Gameplay Abilities（GA）](#concepts-ga)  
> 4.7 [Ability Tasks](#concepts-at)  
> 4.8 [Gameplay Cues（GC）](#concepts-gc)  
> 4.9 [Ability System Globals](#concepts-asg)  
> 4.10 [预测](#concepts-p)

<a name="concepts-asc"></a>
### 4.1 Ability System Component（ASC）
`AbilitySystemComponent` (`ASC`) 是 GAS 的核心。它是一个 `UActorComponent` ([`UAbilitySystemComponent`](https://docs.unrealengine.com/en-US/API/Plugins/GameplayAbilities/UAbilitySystemComponent/index.html))，负责处理与系统的所有交互。任何希望使用 [`GameplayAbilities`](#concepts-ga)、拥有 [`Attributes`](#concepts-a) 或接收 [`GameplayEffects`](#concepts-ge) 的 `Actor` 都必须附加一个 `ASC`。这些对象都位于 `ASC` 内部，并由其管理和复制（`Attributes` 除外，其由 [`AttributeSet`](#concepts-as) 复制）。开发人员应该但不要求对其进行子类化。

附有 `ASC` 的 `Actor` 称为该 `ASC` 的 `OwnerActor`；代表其在游戏世界中实际形态的 `Actor` 称为 `AvatarActor`。`OwnerActor` 和 `AvatarActor` 可以是同一个 `Actor`，例如 MOBA 游戏中的简单 AI 小兵；也可以是不同的 `Actors`。例如，对于玩家控制的 MOBA 英雄，`OwnerActor` 可以是 `PlayerState`，而 `AvatarActor` 是英雄的 `Character`。大多数 `Actors` 会将 `ASC` 放在自身上。如果您的 `Actor` 需要重生，并且要跨重生保留 `Attributes` 或 `GameplayEffects`，那么将 `ASC` 放在 `PlayerState` 上较为合适。

**注意：** 如果您的 `ASC` 位于 `PlayerState` 上，则您需要增加 `PlayerState` 的 `NetUpdateFrequency`。它在 `PlayerState` 上默认为非常低的值，并且可能会在客户端上发生 `Attributes` 和 `GameplayTags` 等更改之前导致延迟或感知滞后。请务必启用 [`Adaptive Network Update Frequency`](https://docs.unrealengine.com/en-US/Gameplay/Networking/Actors/Properties/index.html#adaptivenetworkupdatefrequency)，Fortnite 使用它。

`OwnerActor` 和 `AvatarActor` 如果 `Actors` 不同，则应实现 `IAbilitySystemInterface`。该接口有一个必须重写的函数 `UAbilitySystemComponent* GetAbilitySystemComponent() const`，它返回指向其 `ASC` 的指针。 `ASCs`通过寻找该接口函数来实现系统内部的交互。

`ASC` 将其当前活动的 `GameplayEffects` 保存在 `FActiveGameplayEffectsContainer ActiveGameplayEffects` 中。

`ASC` 将其授予的 `Gameplay Abilities` 保存在 `FGameplayAbilitySpecContainer ActivatableAbilities` 中。每当您计划迭代 `ActivatableAbilities.Items` 时，请务必在循环上方添加 `ABILITYLIST_SCOPE_LOCK();` 以锁定列表以防止更改（由于删除功能）。范围内的每个 `ABILITYLIST_SCOPE_LOCK();` 都会递增 `AbilityScopeLockCount`，然后在超出范围时递减。不要尝试删除`ABILITYLIST_SCOPE_LOCK();`范围内的能力（清除能力功能会在内部检查`AbilityScopeLockCount`以防止在列表被锁定时删除能力）。

<a name="concepts-asc-rm"></a>
### 4.1.1 Replication Mode（复制模式）
`ASC` 定义了三种不同的复制模式来复制 `GameplayEffects`、`GameplayTags` 和 `GameplayCues`：`Full`、`Mixed` 和 `Minimal`。 `Attributes` 是由 `AttributeSet` 复制的。

| 复制模式   | 何时使用                             | 描述                                                                                                                    |
| ------------------ | --------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------ |
| `Full`             | 单人游戏                           | 每个 `GameplayEffect` 都会复制到每个客户端。                                                                          |
| `Mixed`            | 多人游戏，玩家控制 `Actors` | `GameplayEffects` 仅复制到所属客户端。仅 `GameplayTags` 和 `GameplayCues` 被复制给所有人。 |
| `Minimal`          | 多人游戏，人工智能控制 `Actors`     | `GameplayEffects` 永远不会复制给任何人。仅 `GameplayTags` 和 `GameplayCues` 被复制给所有人。           |

**注意：** `Mixed` 复制模式期望 `OwnerActor's` `Owner` 为 `Controller`。默认情况下，`PlayerState's` `Owner` 是 `Controller`，但 `Character's` 不是。如果对 `OwnerActor` 而不是 `PlayerState` 使用 `Mixed` 复制模式，则需要使用有效的 `Controller` 在 `OwnerActor` 上调用 `SetOwner()`。

从 4.24 开始，`PossessedBy()` 现在将 `Pawn` 的所有者设置为新的 `Controller`。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-asc-setup"></a>
### 4.1.2 配置与初始化
`ASCs` 通常在 `OwnerActor's` 构造函数中构造，并显式标记为已复制。 **这必须在 C++ 中完成**。

```c++
AGDPlayerState::AGDPlayerState()
{
	// Create ability system component, and set it to be explicitly replicated
	AbilitySystemComponent = CreateDefaultSubobject<UGDAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	//...
}
```

`ASC` 需要在服务器和客户端上用其 `OwnerActor` 和 `AvatarActor` 进行初始化。您希望在 `Pawn's` `Controller` 设置后（占有后）进行初始化。单人游戏只需要担心服务器路径。

对于 `ASC` 位于 `Pawn` 上的玩家控制角色，我通常在服务器上在 `Pawn's` `PossessedBy()` 函数中进行初始化，并在客户端上在 `PlayerController's` `AcknowledgePossession()` 函数中进行初始化。

```c++
void APACharacterBase::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	// ASC MixedMode replication requires that the ASC Owner's Owner be the Controller.
	SetOwner(NewController);
}
```

```c++
void APAPlayerControllerBase::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	APACharacterBase* CharacterBase = Cast<APACharacterBase>(P);
	if (CharacterBase)
	{
		CharacterBase->GetAbilitySystemComponent()->InitAbilityActorInfo(CharacterBase, CharacterBase);
	}

	//...
}
```

对于 `ASC` 位于 `PlayerState` 上的玩家控制角色，我通常在 `Pawn's` `PossessedBy()` 函数中初始化服务器，并在 `Pawn's` `OnRep_PlayerState()` 函数中在客户端上初始化。这可确保 `PlayerState` 存在于客户端上。

```c++
// Server only
void AGDHeroCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	AGDPlayerState* PS = GetPlayerState<AGDPlayerState>();
	if (PS)
	{
		// Set the ASC on the Server. Clients do this in OnRep_PlayerState()
		AbilitySystemComponent = Cast<UGDAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// AI won't have PlayerControllers so we can init again here just to be sure. No harm in initing twice for heroes that have PlayerControllers.
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, this);
	}
	
	//...
}
```

```c++
// Client only
void AGDHeroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AGDPlayerState* PS = GetPlayerState<AGDPlayerState>();
	if (PS)
	{
		// Set the ASC for clients. Server does this in PossessedBy.
		AbilitySystemComponent = Cast<UGDAbilitySystemComponent>(PS->GetAbilitySystemComponent());

		// Init ASC Actor Info for clients. Server will init its ASC when it possesses a new Actor.
		AbilitySystemComponent->InitAbilityActorInfo(PS, this);
	}

	// ...
}
```

如果您收到错误消息 `LogAbilitySystem: Warning: Can't activate LocalOnly or LocalPredicted ability %s when not local!`，则说明您没有在客户端上初始化 `ASC`。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-gt"></a>
### 4.2 Gameplay Tags
[`FGameplayTags`](https://docs.unrealengine.com/en-US/API/Runtime/GameplayTags/FGameplayTag/index.html) 是在 `GameplayTagManager` 中注册的 `Parent.Child.Grandchild...` 形式的分层名称。这些标签对于分类和描述对象的状态非常有用。例如，如果一个角色被击晕，我们可以在击晕期间给它一个 `State.Debuff.Stun` `GameplayTag`。

您会发现自己用 `GameplayTags` 替换了以前用布尔值或枚举处理的东西，并对对象是否具有某些 `GameplayTags` 进行布尔逻辑。

当给一个对象赋予标签时，我们通常将它们添加到它的 `ASC`（如果它有），以便 GAS 可以与它​​们交互。 `UAbilitySystemComponent` 实现了 `IGameplayTagAssetInterface`，提供了访问其拥有的 `GameplayTags` 的功能。

一个`FGameplayTagContainer`中可以存储多个`GameplayTags`。最好使用 `GameplayTagContainer` 而不是 `TArray<FGameplayTag>`，因为 `GameplayTagContainers` 增加了一些效率魔法。虽然标签是标准的 `FNames`，但如果在项目设置中启用了 `Fast Replication`，则可以将它们有效地打包在 `FGameplayTagContainers` 中进行复制。 `Fast Replication` 要求服务器和客户端具有相同的 `GameplayTags` 列表。这通常不会成为问题，因此您应该启用此选项。 `GameplayTagContainers` 也可以返回一个 `TArray<FGameplayTag>` 进行迭代。

存储在 `FGameplayTagCountContainer` 中的 `GameplayTags` 有一个 `TagMap`，用于存储该 `GameplayTag` 的实例数。 `FGameplayTagCountContainer` 中可能仍包含 `GameplayTag`，但其 `TagMapCount` 为零。如果 `ASC` 仍有 `GameplayTag`，则在调试时可能会遇到这种情况。任何 `HasTag()` 或 `HasMatchingTag()` 或类似函数都将检查 `TagMapCount`，如果 `GameplayTag` 不存在或其 `TagMapCount` 为零，则返回 false。

`GameplayTags` 必须提前在 `DefaultGameplayTags.ini` 中定义。虚幻引擎编辑器在项目设置中提供了一个界面，让开发者可以管理`GameplayTags`，而无需手动编辑`DefaultGameplayTags.ini`。 `GameplayTag` 编辑器可以创建、重命名、搜索引用和删除 `GameplayTags`。

![GameplayTag Editor in Project Settings](https://github.com/tranek/GASDocumentation/raw/master/Images/gameplaytageditor.png)

搜索 `GameplayTag` 引用将在编辑器中显示熟悉的 `Reference Viewer` 图表，其中显示引用 `GameplayTag` 的所有资源。但是，这不会显示任何引用 `GameplayTag` 的 C++ 类。

重命名 `GameplayTags` 会创建重定向，以便仍引用原始 `GameplayTag` 的资产可以重定向到新的 `GameplayTag`。如果可能的话，我更喜欢创建一个新的 `GameplayTag`，手动更新所有对新 `GameplayTag` 的引用，然后删除旧的 `GameplayTag` 以避免创建重定向。

除了 `Fast Replication` 之外，`GameplayTag` 编辑器还可以选择填写通常复制的 `GameplayTags` 以进一步优化它们。

通过 `GameplayEffect` 添加的 `GameplayTags` 会进行网络复制。`ASC` 也允许添加不参与复制的 `LooseGameplayTags`，但需要手动管理它们。示例项目使用 `LooseGameplayTag` 表示 `State.Dead`，使所属客户端能在生命值降到零时立即响应。重生时会手动将 `TagMapCount` 设回零。只有使用 `LooseGameplayTags` 时才应手动调整 `TagMapCount`；更推荐调用 `UAbilitySystemComponent::AddLooseGameplayTag()` 和 `UAbilitySystemComponent::RemoveLooseGameplayTag()`，而不是直接修改 `TagMapCount`。

在 C++ 中获取对 `GameplayTag` 的引用：
```c++
FGameplayTag::RequestGameplayTag(FName("Your.GameplayTag.Name"))
```

对于高级 `GameplayTag` 操作（例如获取父项或子项 `GameplayTags`），请查看 `GameplayTagManager` 提供的功能。要访问 `GameplayTagManager`，请包含 `GameplayTagManager.h` 并用 `UGameplayTagManager::Get().FunctionName` 调用它。 `GameplayTagManager` 实际上将 `GameplayTags` 存储为关系节点（父节点、子节点等），以便比常量字符串操作和比较更快的处理速度。

`GameplayTags` 和 `GameplayTagContainers` 可以具有可选的 `UPROPERTY` 说明符 `Meta = (Categories = "GameplayCue")`，用于过滤蓝图中的标签，以仅显示具有 `GameplayCue` 父标签的 `GameplayTags`。当您知道 `GameplayTag` 或 `GameplayTagContainer` 变量只能用于 `GameplayCues` 时，这非常有用。

或者，有一个名为 `FGameplayCueTag` 的单独结构，它封装了 `FGameplayTag`，并在蓝图中自动过滤 `GameplayTags`，以仅显示父标签为 `GameplayCue` 的标签。

如果要过滤函数中的 `GameplayTag` 参数，请使用 `UFUNCTION` 说明符 `Meta = (GameplayTagFilter = "GameplayCue")`。 `GameplayTagContainer` 函数中的参数无法过滤。如果您想编辑引擎以允许这样做，请查看 `Engine\Plugins\Editor\GameplayTagsEditor\Source\GameplayTagsEditor\Private\SGameplayTagGraphPin.cpp` 中的 `SGameplayTagGraphPin::ParseDefaultValueData()` 如何调用 `FilterString = UGameplayTagsManager::Get().GetCategoriesMetaFromField(PinStructType);` 并将 `FilterString` 传递到 `SGameplayTagGraphPin::GetListContent()` 中的 `SGameplayTagWidget`。 `Engine\Plugins\Editor\GameplayTagsEditor\Source\GameplayTagsEditor\Private\SGameplayTagContainerGraphPin.cpp` 中这些函数的 `GameplayTagContainer` 版本不会检查元字段属性并传递过滤器。

示例项目广泛使用 `GameplayTags`。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-gt-change"></a>
### 4.2.1 响应 Gameplay Tag 的变化
`ASC` 提供了在添加或移除 `GameplayTags` 时触发的 Delegate。其 `EGameplayTagEventType` 参数可以指定：仅在添加或移除 `GameplayTag` 时触发，或在 `GameplayTag's` `TagMapCount` 发生任何变化时触发。

```c++
AbilitySystemComponent->RegisterGameplayTagEvent(FGameplayTag::RequestGameplayTag(FName("State.Debuff.Stun")), EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AGDPlayerState::StunTagChanged);
```

回调函数有一个参数为`GameplayTag`和新的`TagCount`。
```c++
virtual void StunTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
```

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-gt-loadfromplugin"></a>
### 4.2.2 从插件的 .ini 文件加载 Gameplay Tags
如果您使用 `GameplayTags` 创建具有自己的 .ini 文件的插件，则可以在插件的 `StartupModule()` 函数中加载该插件的 `GameplayTag` .ini 目录。

例如，虚幻引擎附带的 CommonConversation 插件就是这样做的：

```c++
void FCommonConversationRuntimeModule::StartupModule()
{
	TSharedPtr<IPlugin> ThisPlugin = IPluginManager::Get().FindPlugin(TEXT("CommonConversation"));
	check(ThisPlugin.IsValid());
	
	UGameplayTagsManager::Get().AddTagIniSearchPath(ThisPlugin->GetBaseDir() / TEXT("Config") / TEXT("Tags"));

	//...
}
```

如果启用了插件，这将在引擎启动时查找目录 `Plugins\CommonConversation\Config\Tags` 并将其中包含 `GameplayTags` 的任何 .ini 文件加载到您的项目中。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-a"></a>
### 4.3 Attributes

<a name="concepts-a-definition"></a>
#### 4.3.1 Attribute 的定义
`Attributes` 是使用 [`FGameplayAttributeData`](https://docs.unrealengine.com/en-US/API/Plugins/GameplayAbilities/FGameplayAttributeData/index.html) 结构定义的 float 值，可以表示角色生命值、等级或药水的剩余使用次数等。只要是属于 `Actor` 的游戏数值，都可以考虑用 `Attribute` 表示。通常应仅通过 [`GameplayEffects`](#concepts-ge) 修改 `Attributes`，以便 ASC 能够[预测](#concepts-p)这些变化。

`Attributes` 定义并存储在 [`AttributeSet`](#concepts-as) 中。`AttributeSet` 负责复制标记为可复制的 `Attributes`。如何定义 `Attributes`，请参阅 [`AttributeSets`](#concepts-as) 一节。

**提示：** 如果您不希望 `Attribute` 显示在 `Attributes` 的编辑器列表中，您可以使用 `Meta = (HideInDetailsView)` `property specifier`。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-a-value"></a>
#### 4.3.2 BaseValue 与 CurrentValue
`Attribute` 由两个值组成 - `BaseValue` 和 `CurrentValue`。 `BaseValue` 是 `Attribute` 的永久值，而 `CurrentValue` 是 `BaseValue` 加上 `GameplayEffects` 的临时修改。例如，您的 `Character` 的移动速度可能为 `Attribute`，`BaseValue` 的移动速度为 600 单位/秒。由于还没有 `GameplayEffects` 修改移动速度，因此 `CurrentValue` 也是 600 u/s。如果她获得临时的 50 u/s 移动速度增益，则 `BaseValue` 保持在 600 u/s 不变，而 `CurrentValue` 现在为 600 + 50，总共 650 u/s。当移动速度增益到期时，`CurrentValue` 恢复为 600 u/s 的 `BaseValue`。

GAS 初学者通常会将 `BaseValue` 与 `Attribute` 的最大值混淆，并尝试将其视为此类。这是一个不正确的做法。可以更改或在能力或 UI 中引用的 `Attributes` 最大值应被视为单独的 `Attributes`。对于硬编码的最大值和最小值，有一种方法可以使用 `FAttributeMetaData` 定义 `DataTable` 来设置最大值和最小值，但 Epic 在结构上方的注释称其为“正在进行的工作”。有关详细信息，请参阅 `AttributeSet.h`。为了防止混淆，我建议将能力或 UI 中可以引用的最大值作为单独的 `Attributes`，并将仅用于固定 `Attributes` 的硬编码最大值和最小值定义为 `AttributeSet` 中的硬编码float。 `Attributes` 的钳位在 `CurrentValue` 的更改的 [PreAttributeChange()](#concepts-as-preattributechange) 中讨论，在 `GameplayEffects` 的 `BaseValue` 的更改的 [PostGameplayEffectExecute()](#concepts-as-postgameplayeffectexecute) 中讨论。

对 `BaseValue` 的永久更改来自 `Instant` `GameplayEffects`，而 `Duration` 和 `Infinite` `GameplayEffects` 更改 `CurrentValue`。定期 `GameplayEffects` 的处理方式与即时 `GameplayEffects` 相同，并更改 `BaseValue`。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-a-meta"></a>
#### 4.3.3 Meta Attributes
某些 `Attributes` 被视为临时值的占位符，旨在与 `Attributes` 交互。这些被称为 `Meta Attributes`。例如，我们通常将损坏定义为 `Meta Attribute`。我们没有使用 `GameplayEffect` 直接改变我们的生命值 `Attribute`，而是使用称为损坏的 `Meta Attribute` 作为占位符。这样，可以在 [`GameplayEffectExecutionCalculation`](#concepts-ge-ec) 中使用 buff 和 debuff 来修改伤害值，并且可以在 `AttributeSet` 中进一步操作，例如从当前护盾 `Attribute` 中减去伤害，然后最终从生命值 `Attribute` 中减去剩余部分。伤害 `Meta Attribute` 在 `GameplayEffects` 之间没有持久性，并且会被每一伤害覆盖。 `Meta Attributes` 通常不会被复制。

`Meta Attributes` 为“我们造成了多少伤害？”之间的伤害和治疗等问题提供了良好的逻辑分离。以及“我们该如何处理这种损坏？”。这种逻辑分离意味着我们的 `Gameplay Effects` 和 `Execution Calculations` 不需要知道目标如何处理损坏。继续我们的损坏示例，`Gameplay Effect` 确定损坏程度，然后 `AttributeSet` 决定如何处理该损坏。并非所有字符都可能具有相同的 `Attributes`，特别是当您使用子类 `AttributeSets` 时。基本 `AttributeSet` 类可能只有生命值 `Attribute`，但子类 `AttributeSet` 可能会添加护盾 `Attribute`。带有防护罩 `Attribute` 的子类 `AttributeSet` 所受到的伤害分布与基础 `AttributeSet` 类不同。

虽然 `Meta Attributes` 是一个很好的设计模式，但它们不是强制性的。如果您只有一个 `Execution Calculation` 用于所有伤害实例，并且所有角色共享一个 `Attribute Set` 类，那么您可以在 `Execution Calculation` 内部对生命值、护盾等进行伤害分配，并直接修改这些 `Attributes`。您只会牺牲灵活性，但这对您来说可能没问题。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-a-changes"></a>
#### 4.3.4 响应 Attribute 的变化
要侦听 `Attribute` 何时发生更改以更新 UI 或其他游戏玩法，请使用 `UAbilitySystemComponent::GetGameplayAttributeValueChangeDelegate(FGameplayAttribute Attribute)`。此函数返回一个您可以绑定的委托，每当 `Attribute` 发生更改时，该委托就会自动调用。委托通过 `NewValue`、`OldValue` 和 `FGameplayEffectModCallbackData` 提供 `FOnAttributeChangeData` 参数。 **注意：** `FGameplayEffectModCallbackData` 只会在服务器上设置。

```c++
AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSetBase->GetHealthAttribute()).AddUObject(this, &AGDPlayerState::HealthChanged);
```

```c++
virtual void HealthChanged(const FOnAttributeChangeData& Data);
```

示例项目绑定到 `GDPlayerState` 上的 `Attribute` 值更改委托，以更新 HUD 并在生命值达到零时响应玩家死亡。

示例项目中包含将其包装到 `ASyncTask` 中的自定义蓝图节点。它在 `UI_HUD` UMG Widget 中用于更新生命值、法力值和耐力值。这个 `AsyncTask` 将永远存在，直到手动调用 `EndTask()`，我们在 UMG Widget 的 `Destruct` 事件中执行此操作。参见 `AsyncTaskAttributeChanged.h/cpp`。

![Listen for Attribute Change BP Node](https://github.com/tranek/GASDocumentation/raw/master/Images/attributechange.png)

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-a-derived"></a>
#### 4.3.5 派生 Attributes
如果某个 `Attribute` 的值需要部分或全部由其他一个或多个 `Attributes` 推导而来，可以使用一个 `Infinite` `GameplayEffect`，并配置一个或多个 `Attribute Based` 或 [`MMC`](#concepts-ge-mmc) 类型的 [`Modifiers`](#concepts-ge-mods)。当所依赖的 `Attribute` 更新时，这个 `Derived Attribute` 也会自动更新。

`Derived Attribute` 上所有 `Modifiers` 的最终公式与 `Modifier Aggregators` 的公式相同。如果您需要按特定顺序进行计算，请在 `MMC` 内完成所有操作。

```
((CurrentValue + Additive) * Multiplicitive) / Division
```

**注意：** 如果在 PIE 中使用多个客户端，您需要在编辑器首选项中禁用 `Run Under One Process`，否则当其独立的 `Attributes` 在除第一个客户端之外的客户端上更新时，`Derived Attributes` 将不会更新。

在此示例中，我们有一个 `Infinite` `GameplayEffect`，它从 `Attributes`、`TestAttrB` 和 `TestAttrC` 导出 `TestAttrA` 的值，公式为 `TestAttrA = (TestAttrA + TestAttrB) * ( 2 * TestAttrC)`。每当任何 `Attributes` 更新其值时，`TestAttrA` 都会自动重新计算其值。

![Derived Attribute Example](https://github.com/tranek/GASDocumentation/raw/master/Images/derivedattribute.png)

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-as"></a>
### 4.4 AttributeSet

<a name="concepts-as-definition"></a>
#### 4.4.1 AttributeSet 的定义
`AttributeSet` 定义、保存和管理对 `Attributes` 的更改。开发人员应从 [`UAttributeSet`](https://docs.unrealengine.com/en-US/API/Plugins/GameplayAbilities/UAttributeSet/index.html) 进行子类化。在 `OwnerActor's` 构造函数中创建 `AttributeSet` 会自动将其注册到 `ASC` 中。 **这必须在 C++ 中完成**。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-as-design"></a>
#### 4.4.2 AttributeSet 的设计
一个`ASC`可以有一个或多个`AttributeSets`。AttributeSet的内存开销可以忽略不计，因此使用多少个 `AttributeSets` 是由开发人员自行决定的。

游戏中的每个 `Actor` 共享一个大型整体 `AttributeSet` 是可以接受的，并且仅在需要时使用属性，而忽略未使用的属性。

或者，您可以选择多个 `AttributeSet` 代表 `Attributes` 分组，您可以根据需要有选择地添加到 `Actors` 中。例如，您可以将 `AttributeSet` 用于健康相关的 `Attributes`，将 `AttributeSet` 用于与法力相关的 `Attributes`，等等。在 MOBA 游戏中，英雄可能需要法力，但小兵可能不需要。因此，英雄将获得法力 `AttributeSet`，而小兵则不会。

此外，`AttributeSets` 可以被细分为另一种选择性选择 `Actor` 所具有的 `Attributes` 的方法。 `Attributes` 在内部称为 `AttributeSetClassName.AttributeName`。当您对 `AttributeSet` 进行子类化时，父类中的所有 `Attributes` 仍将以父类的名称作为前缀。

虽然您可以拥有多个 `AttributeSet`，但在 `ASC` 上不应拥有多个同一类别的 `AttributeSet`。如果您有多个同一类别的 `AttributeSet`，它不会知道要使用哪一个 `AttributeSet`，只会选择一个。

<a name="concepts-as-design-subcomponents"></a>
##### 4.4.2.1 具有独立 Attributes 的子组件
在 `Pawn` 上有多个可损坏组件（例如单独可损坏装甲件）的情况下，我建议如果您知道 `Pawn` 可能必须在一个 `AttributeSet` 上制造那么多生命值 `Attributes` 的可损坏组件的最大数量 - DamageableCompHealth0、DamageableCompHealth1 等来表示这些可损坏组件的逻辑“插槽”。在可损坏组件类实例中，分配可由 `GameplayAbilities` 或 [`Executions`](#concepts-ge-ec) 读取的插槽号 `Attribute`，以了解要对哪个 `Attribute` 施加损坏。 `Pawns` 的可损坏组件数量少于最大数量或为零即可。仅仅因为 `AttributeSet` 有 `Attribute`，并不意味着您必须使用它。未使用的 `Attributes` 占用少量内存。

如果您的子组件每个都需要许多 `Attributes`，则可能存在无限数量的子组件，子组件可以分离并由其他玩家（例如武器）使用，或者由于任何其他原因此方法不适合您，我建议您放弃 `Attributes` 并在组件上存储普通的旧浮标。参见 [物品属性](#concepts-as-design-itemattributes)。

<a name="concepts-as-design-addremoveruntime"></a>
##### 4.4.2.2 在运行时添加和移除 AttributeSet
`AttributeSets` 可以在运行时添加和删除 `ASC`；但是，删除 `AttributeSets` 可能很危险。例如，如果在服务器之前从客户端删除 `AttributeSet`，并将 `Attribute` 值更改复制到客户端，则 `Attribute` 将找不到其 `AttributeSet` 并导致游戏崩溃。

将武器添加到库存中：
```c++
AbilitySystemComponent->GetSpawnedAttributes_Mutable().AddUnique(WeaponAttributeSetPointer);
AbilitySystemComponent->ForceReplication();
```

从库存中移除武器时：
```c++
AbilitySystemComponent->GetSpawnedAttributes_Mutable().Remove(WeaponAttributeSetPointer);
AbilitySystemComponent->ForceReplication();
```
<a name="concepts-as-design-itemattributes"></a>
##### 4.4.2.3 物品的 Attributes（武器弹药）
有几种方法可以使用 `Attributes` 来实现可装备的物品（武器弹药、装甲耐久度等）。所有这些方法都将值直接存储在项目上。对于在其生命周期内可由多名玩家装备的物品来说，这是必要的。

> 1. 在物品上使用普通 float（**推荐**）
> 1. 项目上单独的 `AttributeSet`
> 1. 项目上单独的 `ASC`

<a name="concepts-as-design-itemattributes-plainfloats"></a>
###### 4.4.2.3.1 在物品上使用普通 float
在项目类实例上存储普通float 值，而不是 `Attributes`。 Fortnite 和 [GASShooter](https://github.com/tranek/GASShooter) 以这种方式处理枪支弹药。对于枪，将最大弹夹尺寸、弹夹中的当前弹药、储备弹药等直接存储为枪实例上的复制float (`COND_OwnerOnly`)。如果武器共享备用弹药，您可以将备用弹药作为共享弹药 `AttributeSet` 中的 `Attribute` 移动到角色上（重装能力可以使用 `Cost GE` 将备用弹药拉入枪的浮动弹夹弹药中）。由于您没有将 `Attributes` 用于当前弹夹弹药，因此您需要重写 `UGameplayAbility` 中的一些功能来检查枪上的浮标并应用成本。在授予该能力时将[`GameplayAbilitySpec`](https://github.com/tranek/GASDocumentation#concepts-ga-spec)中的枪设为`SourceObject`意味着您将可以在该能力内使用授予该能力的枪。

为了防止枪在自动射击期间复制回弹药量并破坏本地弹药量，请在玩家在 `PreReplication()` 中拥有 `IsFiring` `GameplayTag` 时禁用复制。您实际上是在这里进行自己的本地预测。

```c++
void AGSWeapon::PreReplication(IRepChangedPropertyTracker& ChangedPropertyTracker)
{
	Super::PreReplication(ChangedPropertyTracker);

	DOREPLIFETIME_ACTIVE_OVERRIDE(AGSWeapon, PrimaryClipAmmo, (IsValid(AbilitySystemComponent) && !AbilitySystemComponent->HasMatchingGameplayTag(WeaponIsFiringTag)));
	DOREPLIFETIME_ACTIVE_OVERRIDE(AGSWeapon, SecondaryClipAmmo, (IsValid(AbilitySystemComponent) && !AbilitySystemComponent->HasMatchingGameplayTag(WeaponIsFiringTag)));
}
```

好处：
1. 避免使用 `AttributeSets` 的限制（见下文）

限制：
1. 无法使用现有的 `GameplayEffect` 工作流程（`Cost GEs` 用于弹药使用等）
1. 需要覆盖 `UGameplayAbility` 上的关键功能，以检查枪支的浮动并应用弹药成本

<a name="concepts-as-design-itemattributes-attributeset"></a>
###### 4.4.2.3.2 在物品上使用 `AttributeSet`
在获得 [added to the player's `ASC` on adding it to the player's inventory](#concepts-as-design-addremoveruntime) 的物品上使用单独的 `AttributeSet` 是可行的，但它有一些主要限制。我在 [GASShooter](https://github.com/tranek/GASShooter) 的早期版本中使用了这个作为武器弹药。该武器将其 `Attributes` 存储在武器类别中的 `AttributeSet` 中，例如最大弹夹尺寸、弹夹中的当前弹药、储备弹药等。如果武器共享备用弹药，您可以将备用弹药移动到共享弹药 `AttributeSet` 中的角色上。当武器被添加到服务器上的玩家库存时，该武器会将其 `AttributeSet` 添加到玩家的 `ASC::SpawnedAttributes` 中。然后服务器会将其复制到客户端。如果该武器从库存中移除，则会从 `ASC::SpawnedAttributes` 中移除其 `AttributeSet`。

当 `AttributeSet` 存在于 `OwnerActor` 之外的其他东西（比如武器）上时，您最初会在 `AttributeSet` 中遇到一些编译错误。修复方法是在 `BeginPlay()` 中而不是在构造函数中构造 `AttributeSet`，并在武器上实现 `IAbilitySystemInterface`（将武器添加到玩家库存时将指针设置为 `ASC`）。

```c++
void AGSWeapon::BeginPlay()
{
	if (!AttributeSet)
	{
		AttributeSet = NewObject<UGSWeaponAttributeSet>(this);
	}
	//...
}
```

您可以通过查看这个 [旧版本的 GASShooter](https://github.com/tranek/GASShooter/tree/df5949d0dd992bd3d76d4a728f370f2e2c827735) 在实践中看到它。

好处：
1. 可以使用现有的 `GameplayAbility` 和 `GameplayEffect` 工作流程（`Cost GEs` 用于弹药使用等）
1. 对于非常小的一组项目来说设置简单

限制：
1. 您必须为每种武器类型创建一个新的 `AttributeSet` 类别。 `ASCs` 在功能上只能具有类的一个 `AttributeSet` 实例，因为对 `Attribute` 的更改会在 `ASCs` `SpawnedAttributes` 数组中查找其 `AttributeSet` 类的第一个实例。同一 `AttributeSet` 类的其他实例将被忽略。
1. 由于先前每个 `AttributeSet` 类别有一个 `AttributeSet` 实例的原因，玩家的库存中每种类型的武器只能拥有一种。
1. 删除 `AttributeSet` 是危险的。在 GASShooter 中，如果玩家用火箭自杀，玩家会立即从库存中移除火箭发射器（包括 `ASC` 中的 `AttributeSet`）。当服务器复制火箭发射器的弹药`Attribute`发生变化时，客户端的`ASC`上不再存在`AttributeSet`，游戏崩溃。

<a name="concepts-as-design-itemattributes-asc"></a>
###### 4.4.2.3.3 在物品上使用 `ASC`
为每件物品都配置一个完整的 `AbilitySystemComponent` 是一种极端方案。我自己没有这样做过，也没有见过实际项目采用这种方式。要让它正常工作，需要投入大量工程工作。

> 是否可以拥有多个具有相同所有者但不同Avatar的AbilitySystemComponent（例如，在将所有者设置为 PlayerState 的Pawn和武器/物品/射弹上）？
> 
> 我看到的第一个问题是在拥有的 Actor 上实现 IGameplayTagAssetInterface 和 IAbilitySystemInterface。前者可能是可能的：只需聚合所有 ASC 的标签（但要注意 -HasAllMatchingGameplayTags 可能只能通过跨 ASC 聚合来满足。仅将调用转发到每个 ASC 并将结果或在一起是不够的）。但后者更加棘手：哪一个 ASC 才是权威的？如果有人想申请 GE - 哪一个应该获得它？也许你可以解决这些问题，但问题的这一面将是最难的：所有者下面将有多个 ASC。
> 
> 不过，Pawn和武器上的单独 ASC 本身就有意义。例如，区分描述武器的标签和描述拥有Pawn的标签。也许赋予武器的标签也“适用于”所有者而不是其他东西确实有意义（例如，属性和 GE 是独立的，但所有者将像我上面描述的那样聚合拥有的标签）。我确信这会成功。但同一所有者拥有多个 ASC 可能会很冒险。

*Epic 的 Dave Ratti 对 [社区问题 #6](https://epicgames.ent.box.com/s/m1egifkxv3he3u3xezb9hzbgroxyhx89)* 的回答

好处：
1. 可以使用现有的 `GameplayAbility` 和 `GameplayEffect` 工作流程（`Cost GEs` 用于弹药使用等）
1. 可以重复使用 `AttributeSet` 类（每种武器的 ASC 上有一个）

限制：
1. 工程成本未知
1. 有可能吗？

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-as-attributes"></a>
#### 4.4.3 定义 Attributes
**`Attributes` 只能在 C++** 中的 `AttributeSet's` 头文件中定义。建议将此宏块添加到每个 `AttributeSet` 头文件的顶部。它将自动为您的 `Attributes` 生成 getter 和 setter 函数。

```c++
// Uses macros from AttributeSet.h
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
```

支持网络复制的生命值 Attribute 可以这样定义：

```c++
UPROPERTY(BlueprintReadOnly, Category = "Health", ReplicatedUsing = OnRep_Health)
FGameplayAttributeData Health;
ATTRIBUTE_ACCESSORS(UGDAttributeSetBase, Health)
```

还在头文件中定义 `OnRep` 函数：
```c++
UFUNCTION()
virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
```

`AttributeSet` 的 .cpp 文件应使用预测系统使用的 `GAMEPLAYATTRIBUTE_REPNOTIFY` 宏填充 `OnRep` 函数：
```c++
void UGDAttributeSetBase::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UGDAttributeSetBase, Health, OldHealth);
}
```

最后，需要将`Attribute`添加到`GetLifetimeReplicatedProps`：
```c++
void UGDAttributeSetBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UGDAttributeSetBase, Health, COND_None, REPNOTIFY_Always);
}
```

`REPNOTIFY_Always` 告诉 `OnRep` 函数在本地值已经等于从服务器复制的值（由于预测）时触发。默认情况下，如果本地值与从服务器复制的值相同，则不会触发 `OnRep` 函数。

如果 `Attribute` 不像 `Meta Attribute` 那样复制，则可以跳过 `OnRep` 和 `GetLifetimeReplicatedProps` 步骤。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-as-init"></a>
#### 4.4.4 初始化 Attributes
有多种方法可以初始化 `Attributes`（将其 `BaseValue` 以及 `CurrentValue` 设置为某个初始值）。 Epic 建议使用即时 `GameplayEffect`。这也是示例项目中使用的方法。

请参阅示例项目中的 `GE_HeroAttributes` 蓝图，了解如何制作即时 `GameplayEffect` 来初始化 `Attributes`。 `GameplayEffect` 的应用发生在 C++ 中。

如果定义 `Attributes` 时使用了 `ATTRIBUTE_ACCESSORS` 宏，那么 `AttributeSet` 会为每个 `Attribute` 自动生成初始化函数，您可以按需在 C++ 中调用。

```c++
// InitHealth(float InitialValue) is an automatically generated function for an Attribute 'Health' defined with the `ATTRIBUTE_ACCESSORS` macro
AttributeSet->InitHealth(100.0f);
```

有关初始化 `Attributes` 的更多方法，请参阅 `AttributeSet.h`。

**注意：** 在 4.24 之前，`FAttributeSetInitterDiscreteLevels` 不支持 `FGameplayAttributeData`。它创建于 `Attributes` 仍使用普通 float 的时期，因此会报错，指出 `FGameplayAttributeData` 不是 `Plain Old Data`（`POD`）。此问题已在 4.24 修复，详见 https://issues.unrealengine.com/issue/UE-76557.

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-as-preattributechange"></a>
#### 4.4.5 PreAttributeChange()
`PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)` 是 `AttributeSet` 中用于在属性值改变前响应变化的主要函数之一，处理的是 `Attribute's` `CurrentValue`。可以在这里通过引用参数 `NewValue`，对即将写入的 `CurrentValue` 进行限幅（clamp）。

例如，要限制移动速度Modifier，示例项目会这样做：
```c++
if (Attribute == GetMoveSpeedAttribute())
{
	// Cannot slow less than 150 units/s and cannot boost more than 1000 units/s
	NewValue = FMath::Clamp<float>(NewValue, 150, 1000);
}
```
`GetMoveSpeedAttribute()` 函数是由我们添加到 `AttributeSet.h` ([定义 Attributes](#concepts-as-attributes)) 的宏块创建的。

这是由对 `Attributes` 的任何更改触发的，无论是使用 `Attribute` 设置器（由 `AttributeSet.h` ([定义 Attributes](#concepts-as-attributes)) 中的宏块定义）还是使用 [`GameplayEffects`](#concepts-ge)。

**注意：** 此处发生的任何钳制都不会永久更改 `ASC` 上的Modifier。它仅更改查询修饰符返回的值。这意味着从 [`GameplayEffectExecutionCalculations`](#concepts-ge-ec) 和 [`ModifierMagnitudeCalculations`](#concepts-ge-mmc) 等所有Modifier重新计算 `CurrentValue` 的任何操作都需要再次实施钳位。

**注意：** Epic 对 `PreAttributeChange()` 的评论说不要将其用于游戏事件，而是主要用于夹紧。 `Attribute` 更改上的游戏事件的推荐位置是 `UAbilitySystemComponent::GetGameplayAttributeValueChangeDelegate(FGameplayAttribute Attribute)` ([响应 Attribute 的变化](#concepts-a-changes))。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-as-postgameplayeffectexecute"></a>
#### 4.4.6 PostGameplayEffectExecute()
`PostGameplayEffectExecute(const FGameplayEffectModCallbackData & Data)` 仅在从即时 [`GameplayEffect`](#concepts-ge) 更改为 `Attribute` 的 `BaseValue` 后触发。当它们从 `GameplayEffect` 更改时，这是进行更多 `Attribute` 操作的有效位置。

例如，在示例项目中，我们从此处的生命值 `Attribute` 中减去最终伤害 `Meta Attribute`。如果有盾牌 `Attribute`，我们会先减去它的伤害，然后再减去生命值的剩余部分。示例项目还使用此位置来应用命中反应动画、显示浮动伤害数字以及为杀手分配经验和黄金赏金。根据设计，伤害 `Meta Attribute` 将始终通过即时 `GameplayEffect` 而不会通过 `Attribute` 设置器。

其他`Attributes`那只会有他们的`BaseValue`从瞬间改变`GameplayEffects`像法力和耐力一样，也可以限制为其对应的最大值`Attributes`这里。

**注意：** 当调用 `PostGameplayEffectExecute()` 时，对 `Attribute` 的更改已经发生，但它们尚未复制回客户端，因此此处的限制值不会导致客户端进行两次网络更新。客户端只有在锁定后才会收到更新。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-as-onattributeaggregatorcreated"></a>
#### 4.4.7 OnAttributeAggregatorCreated()
当系统为此集合中的 `Attribute` 创建 `Aggregator` 时，会调用 `OnAttributeAggregatorCreated(const FGameplayAttribute& Attribute, FAggregator* NewAggregator)`。您可以在这里自定义 [`FAggregatorEvaluateMetaData`](https://docs.unrealengine.com/en-US/API/Plugins/GameplayAbilities/FAggregatorEvaluateMetaData/index.html)。`Aggregator` 会使用 `AggregatorEvaluateMetaData`，根据应用的所有 [`Modifiers`](#concepts-ge-mods) 计算 `Attribute` 的 `CurrentValue`。默认情况下，`AggregatorEvaluateMetaData` 只用于决定哪些 `Modifiers` 有资格参与计算。下面的 `MostNegativeMod_AllPositiveMods` 示例允许所有正向 `Modifiers` 参与，但负向 `Modifiers` 中只允许数值最负的一个参与。Paragon 利用这一机制，使玩家同时受到多个减速效果时仅承受最强的减速，同时仍应用全部移速增益。不符合条件的 `Modifiers` 仍保留在 `ASC` 中，只是不参与最终 `CurrentValue` 的聚合。条件改变后，它们仍可能获得资格；例如最强的负向 `Modifier` 到期时，下一个最强的负向 `Modifier`（如果存在）就会参与计算。

在只允许最负的 `Modifier` 和所有正的 `Modifiers` 的示例中使用 AggregatorEvaluateMetaData：

```c++
virtual void OnAttributeAggregatorCreated(const FGameplayAttribute& Attribute, FAggregator* NewAggregator) const override;
```

```c++
void UGSAttributeSetBase::OnAttributeAggregatorCreated(const FGameplayAttribute& Attribute, FAggregator* NewAggregator) const
{
	Super::OnAttributeAggregatorCreated(Attribute, NewAggregator);

	if (!NewAggregator)
	{
		return;
	}

	if (Attribute == GetMoveSpeedAttribute())
	{
		NewAggregator->EvaluationMetaData = &FAggregatorEvaluateMetaDataLibrary::MostNegativeMod_AllPositiveMods;
	}
}
```

您的自定义限定符 `AggregatorEvaluateMetaData` 应作为静态变量添加到 `FAggregatorEvaluateMetaDataLibrary` 中。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge"></a>
### 4.5 Gameplay Effects（GE）

<a name="concepts-ge-definition"></a>
#### 4.5.1 Gameplay Effect 的定义
[`GameplayEffects`](https://docs.unrealengine.com/en-US/API/Plugins/GameplayAbilities/UGameplayEffect/index.html)（`GE`）是[`Attributes`](#concepts-a)和[`GameplayTags`](#concepts-gt)对自己和他人能力进行改变的容器。它们可以立即导致 `Attribute` 变化，例如伤害或治疗，或应用长期状态增益/减益，例如移动速度提升或眩晕。 `UGameplayEffect` 类是一个**仅数据**类，它定义了单个GameplayEffect。不应向 `GameplayEffects` 添加额外的逻辑。通常，设计人员会创建 `UGameplayEffect` 的许多蓝图子类。

`GameplayEffects` 将 `Attributes` 更改为 [`Modifiers`](#concepts-ge-mods) 和 [`Executions` (`GameplayEffectExecutionCalculation`)](#concepts-ge-ec)。

`GameplayEffects` 具有三种类型的持续时间：`Instant`、`Duration` 和 `Infinite`。

此外，`GameplayEffects` 可以添加或执行 [`GameplayCues`](#concepts-gc)。`Instant` `GameplayEffect` 会对其 `GameplayCue` `GameplayTags` 调用 `Execute`；`Duration` 或 `Infinite` `GameplayEffect` 则会对其 `GameplayCue` `GameplayTags` 调用 `Add` 和 `Remove`。

| 持续时间类型 | GameplayCue事件 | 何时使用                                                                                                                                                                                                                                |
| ------------- | ----------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `Instant`     | 执行           | 用于立即永久更改 `Attribute's` `BaseValue`。 `GameplayTags` 不会被应用，即使对于框架也是如此。                                                                                                                    |
| `Duration`    | 添加和删​​除      | 用于对 `Attribute's` `CurrentValue` 进行临时更改并应用 `GameplayTags`，当 `GameplayEffect` 过期或手动删除时，该更改将被删除。持续时间在 `UGameplayEffect` 类/蓝图中指定。       |
| `Infinite`    | 添加和删​​除      | 用于对 `Attribute's` `CurrentValue` 进行临时更改并应用 `GameplayTags`，当删除 `GameplayEffect` 时，该更改将被删除。它们永远不会自行过期，必须通过能力或 `ASC` 手动删除。 |

`Duration` 和 `Infinite` `GameplayEffects` 可以选择应用 `Periodic Effects`，从而按照其 `Period` 的定义，每隔 `X` 秒应用其 `Modifiers` 和 `Executions`。在更改 `Attribute's`、`BaseValue` 和 `Executing` `GameplayCues` 时，`Periodic Effects` 被视为 `Instant` `GameplayEffects`。这些对于随时间推移造成的伤害 (DOT) 类型的效果非常有用。 **注意：** `Periodic Effects` 不能是 [predicted](#concepts-p)。

`Duration`和`Infinite` `GameplayEffects`如果其`Ongoing Tag Requirements`不满足/满足（[Gameplay Effect 的 Tags](#concepts-ge-tags)），可以在申请后暂时关闭和打开。关闭 `GameplayEffect` 会消除其 `Modifiers` 和应用的 `GameplayTags` 的影响，但不会消除 `GameplayEffect`。重新打开 `GameplayEffect` 会重新应用其 `Modifiers` 和 `GameplayTags`。

如果您需要手动重新计算 `Duration` 或 `Infinite` `GameplayEffect` 的 `Modifiers`（假设您有一个 `MMC` 使用的数据不是来自 `Attributes`），您可以使用与它已有的相同级别来调用 `UAbilitySystemComponent::ActiveGameplayEffects.SetActiveGameplayEffectLevel(FActiveGameplayEffectHandle ActiveHandle, int32 NewLevel)` `UAbilitySystemComponent::ActiveGameplayEffects.GetActiveGameplayEffect(ActiveHandle).Spec.GetLevel()`。当支持 `Attributes` 更新时，基于支持 `Attributes` 的 `Modifiers` 会自动更新。 `SetActiveGameplayEffectLevel()`更新`Modifiers`的主要功能是：

```C++
MarkItemDirty(Effect);
Effect.Spec.CalculateModifierMagnitudes();
// Private function otherwise we'd call these three functions without needing to set the level to what it already is
UpdateAllAggregatorModMagnitudes(Effect);
```

`GameplayEffects` 通常不被实例化。当能力或`ASC`想要应用`GameplayEffect`时，它会从`GameplayEffect's` `ClassDefaultObject`创建[`GameplayEffectSpec`](#concepts-ge-spec)。成功应用的 `GameplayEffectSpecs` 然后会添加到名为 `FActiveGameplayEffect` 的新结构中，这是 `ASC` 在名为 `ActiveGameplayEffects` 的特殊容器结构中跟踪的内容。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge-applying"></a>
#### 4.5.2 应用 Gameplay Effects
`GameplayEffects`可以通过[`GameplayAbilities`](#concepts-ga)上的功能和`ASC`上的功能以多种方式应用，通常采用`ApplyGameplayEffectTo`的形式。不同的函数本质上是便利函数，最终将在 `Target` 上调用 `UAbilitySystemComponent::ApplyGameplayEffectSpecToSelf()`。

要在 `GameplayAbility` 之外应用 `GameplayEffects`（例如从射弹），您需要获取 `Target's` `ASC` 并将其功能之一用于 `ApplyGameplayEffectToSelf`。

您可以通过绑定到其委托来侦听何时将任何 `Duration` 或 `Infinite` `GameplayEffects` 应用于 `ASC`：
```c++
AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(this, &APACharacterBase::OnActiveGameplayEffectAddedCallback);
```
回调函数：
```c++
virtual void OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);
```

无论复制模式如何，服务器都将始终调用此函数。自治代理只会在 `Full` 和 `Mixed` 复制模式下为复制的 `GameplayEffects` 调用此方法。模拟代理只会在 `Full` [复制模式](#concepts-asc-rm) 中调用它。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ga-removing"></a>
#### 4.5.3 移除 Gameplay Effects
`GameplayEffects` 可以通过多种方式从 [`GameplayAbilities`](#concepts-ga) 上的函数和 `ASC` 上的函数中删除，通常采用 `RemoveActiveGameplayEffect` 的形式。不同的函数本质上是便利函数，最终将在 `Target` 上调用 `FActiveGameplayEffectsContainer::RemoveActiveEffects()`。

要从 `GameplayAbility` 外部删除 `GameplayEffects`，您需要获取 `Target's` `ASC` 并将其功能之一用于 `RemoveActiveGameplayEffect`。

您可以通过绑定到其委托来侦听何时从 `ASC` 中删除任何 `Duration` 或 `Infinite` `GameplayEffects`：
```c++
AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(this, &APACharacterBase::OnRemoveGameplayEffectCallback);
```
回调函数：
```c++
virtual void OnRemoveGameplayEffectCallback(const FActiveGameplayEffect& EffectRemoved);
```

无论复制模式如何，服务器都将始终调用此函数。自治代理只会在 `Full` 和 `Mixed` 复制模式下为复制的 `GameplayEffects` 调用此方法。模拟代理只会在 `Full` [复制模式](#concepts-asc-rm) 中调用它。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge-mods"></a>
#### 4.5.4 Gameplay Effect 的 Modifiers
`Modifiers` 更改 `Attribute` 是 [predictively](#concepts-p) 更改 `Attribute` 的唯一方法。 `GameplayEffect` 可以有零个或多个 `Modifiers`。每个`Modifier`只负责通过指定的操作改变一个`Attribute`。

| 手术  | 描述                                                                                                         |
| ---------- | ------------------------------------------------------------------------------------------------------------------- |
| `Add`      | 将结果添加到 `Modifier's` 指定的 `Attribute` 中。使用负值进行减法。                    |
| `Multiply` | 将结果乘以 `Modifier's` 指定的 `Attribute`。                                                    |
| `Divide`   | 将结果除以 `Modifier's` 指定的 `Attribute`。                                                  |
| `Override` | 用结果覆盖 `Modifier's` 指定的 `Attribute`。                                                   |

`Attribute` 的 `CurrentValue` 是其所有 `Modifiers` 添加到其 `BaseValue` 的聚合结果。 `Modifiers`如何聚合的公式在`GameplayEffectAggregator.cpp`中的`FAggregatorModChannel::EvaluateWithBase`中定义如下：
```c++
((InlineBaseValue + Additive) * Multiplicitive) / Division
```

任何 `Override` `Modifiers` 将覆盖最终值，最后应用的 `Modifier` 优先。

**注意：** 对于基于百分比的更改，请确保使用 `Multiply` 操作，以便在添加后发生。

**注意：** [预测](#concepts-p) 在百分比变化方面遇到问题。

`Modifiers` 有四种类型：可扩展浮点型、基于属性、自定义计算类和由调用者设置。它们都会生成一些float 值，然后用于根据其操作更改 `Modifier` 的指定 `Attribute`。

| `Modifier`型            | 描述                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           |
| -------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `Scalable Float`           | `FScalableFloats` 是一个可以指向数据表的结构，该数据表的变量为行，级别为列。可扩展浮点将自动读取能力当前级别（或如果在 [`GameplayEffectSpec`](#concepts-ge-spec) 上覆盖则为不同级别）的指定表行的值。该值可以进一步通过系数来操纵。如果未指定数据表/行，则会将该值视为 1，因此该系数可用于在所有级别将其硬编码为单个值。 ![ScalableFloat](https://github.com/tranek/GASDocumentation/raw/master/Images/scalablefloats.png)                                                                                                                                                                                                                                                                                                                                                                                                                                                       |
| `Attribute Based`          | `Attribute Based` `Modifiers` 将 `Source`（创建了 `GameplayEffectSpec`）或 `Target`（接收了 `GameplayEffectSpec`）上的支持 `Attribute` 的 `CurrentValue` 或 `BaseValue` 并进一步修改为系数以及前后系数相加。 `Snapshotting` 表示创建 `GameplayEffectSpec` 时捕获支持 `Attribute`，而无快照表示应用 `GameplayEffectSpec` 时捕获 `Attribute`。                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     |
| `Custom Calculation Class` | `Custom Calculation Class` 为复杂的 `Modifiers` 提供了最大的灵活性。该 `Modifier` 采用 [`ModifierMagnitudeCalculation`](#concepts-ge-mmc) 类，并且可以通过系数以及前后系数加法进一步操作生成的float 值。                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    |
| `Set By Caller`            | `SetByCaller` `Modifiers` 是运行时由在 `GameplayEffectSpec` 上创建 `GameplayEffectSpec` 的能力或人员在 `GameplayEffect` 外部设置的值。例如，如果您想根据玩家按住按钮充能技能的时间来设置伤害，则可以使用 `SetByCaller`。 `SetByCallers` 本质上是驻留在 `GameplayEffectSpec` 上的 `TMap<FGameplayTag, float>`。 `Modifier` 只是告诉 `Aggregator` 查找与提供的 `GameplayTag` 关联的 `SetByCaller` 值。 `Modifiers`使用的`SetByCallers`只能使用`GameplayTag`版本的概念。 `FName` 版本在此被禁用。如果 `Modifier` 设置为 `SetByCaller`，但 `GameplayEffectSpec` 上不存在具有正确 `GameplayTag` 的 `SetByCaller`，则游戏将引发运行时错误并返回值 0。这可能会在 `Divide` 操作的情况下导致问题。有关如何使用 `SetByCallers` 的更多信息，请参阅 [`SetByCallers`](#concepts-ge-spec-setbycaller)。 |

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge-mods-multiplydivide"></a>
##### 4.5.4.1 Multiply 和 Divide Modifiers
默认情况下，所有 `Multiply` 和 `Divide` `Modifiers` 都会先相加，然后再将它们乘或除到 `Attribute` 的 `BaseValue` 中。

```c++
float FAggregatorModChannel::EvaluateWithBase(float InlineBaseValue, const FAggregatorEvaluateParameters& Parameters) const
{
	...
	float Additive = SumMods(Mods[EGameplayModOp::Additive], GameplayEffectUtilities::GetModifierBiasByModifierOp(EGameplayModOp::Additive), Parameters);
	float Multiplicitive = SumMods(Mods[EGameplayModOp::Multiplicitive], GameplayEffectUtilities::GetModifierBiasByModifierOp(EGameplayModOp::Multiplicitive), Parameters);
	float Division = SumMods(Mods[EGameplayModOp::Division], GameplayEffectUtilities::GetModifierBiasByModifierOp(EGameplayModOp::Division), Parameters);
	...
	return ((InlineBaseValue + Additive) * Multiplicitive) / Division;
	...
}
```

```c++
float FAggregatorModChannel::SumMods(const TArray<FAggregatorMod>& InMods, float Bias, const FAggregatorEvaluateParameters& Parameters)
{
	float Sum = Bias;

	for (const FAggregatorMod& Mod : InMods)
	{
		if (Mod.Qualifies())
		{
			Sum += (Mod.EvaluatedMagnitude - Bias);
		}
	}

	return Sum;
}
```
*来自 `GameplayEffectAggregator.cpp`*

在此公式中，`Multiply` 和 `Divide` `Modifiers` 的 `Bias` 值为 `1`（`Addition` 的 `Bias` 为 `0`）。所以它看起来像：

```
1 + (Mod1.Magnitude - 1) + (Mod2.Magnitude - 1) + ...
```

这个公式会导致一些意想不到的结果。首先，该公式将所有修饰符相加，然后将它们相乘或除以 `BaseValue`。大多数人会期望它将它们相乘或相除。例如，如果您有 `1.5` 的两个 `Multiply` 修饰符，大多数人会期望 `BaseValue` 乘以 `1.5 x 1.5 = 2.25`。相反，这会将 `1.5` 加在一起，将 `BaseValue` 乘以 `2` (`50% increase + another 50% increase = 100% increase`)。这是来自 `GameplayPrediction.h` 的示例，`500` 上的 `10%` 速度增益将是 `550`。添加另一个 `10%` 速度增益，它将是 `600`。

其次，该公式对于可以使用哪些值有一些未记录的规则，因为它是在设计时考虑到 Paragon 的。

`Multiply`和`Divide`乘加公式的规则：
* `(No more than one value < 1) AND (Any number of values [1, 2))`
* `OR (One value >= 2)`

公式中的`Bias`基本上是减去`[1, 2)`范围内的数字的整数位。第一个 `Modifier` 的 `Bias` 从起始 `Sum` 值中减去（在循环之前设置为 `Bias`），这就是为什么任何值本身都可以工作，以及为什么一个值 `< 1` 将与 `[1, 2)` 范围内的数字一起工作。

`Multiply` 的一些示例：  
乘数：`0.5`  
`1 + (0.5 - 1) = 0.5`，正确

乘数：`0.5, 0.5`  
`1 + (0.5 - 1) + (0.5 - 1) = 0`, incorrect expected `1`?小于 `1` 的多个值对于添加乘数没有意义。 Paragon 设计为仅使用 [greatest negative value for `Multiply` `Modifiers`](#cae-nonstackingge)，因此最多只有一个小于 `1` 的值乘以 `BaseValue`。

乘数：`1.1, 0.5`  
`1 + (0.5 - 1) + (1.1 - 1) = 0.6`，正确

乘数：`5, 5`  
`1 + (5 - 1) + (5 - 1) = 9`，错误的预期 `10`。永远是 `sum of the Modifiers - number of Modifiers + 1`。

许多游戏会希望它们的 `Multiply` 和 `Divide` `Modifiers` 在应用于 `BaseValue` 之前先进行乘法和除法运算。为此，您需要 **更改 `FAggregatorModChannel::EvaluateWithBase()` 的引擎代码**。

```c++
float FAggregatorModChannel::EvaluateWithBase(float InlineBaseValue, const FAggregatorEvaluateParameters& Parameters) const
{
	...
	float Multiplicitive = MultiplyMods(Mods[EGameplayModOp::Multiplicitive], Parameters);
	float Division = MultiplyMods(Mods[EGameplayModOp::Division], Parameters);
	...

	return ((InlineBaseValue + Additive) * Multiplicitive) / Division;
}
```

```c++
float FAggregatorModChannel::MultiplyMods(const TArray<FAggregatorMod>& InMods, const FAggregatorEvaluateParameters& Parameters)
{
	float Multiplier = 1.0f;

	for (const FAggregatorMod& Mod : InMods)
	{
		if (Mod.Qualifies())
		{
			Multiplier *= Mod.EvaluatedMagnitude;
		}
	}

	return Multiplier;
}
```

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge-mods-gameplaytags"></a>
##### 4.5.4.2 Modifier 上的 Gameplay Tags

可为每个 [Modifier](#concepts-ge-mods) 设置 `SourceTags` 和 `TargetTags`。它们的工作原理与 `GameplayEffect` 的 [`Application Tag requirements`](#concepts-ge-tags) 相同。因此，仅在应用效果时才考虑标签。 IE。当具有周期性、无限效果时，仅在第一次应用该效果时考虑它们，但*不*在每次周期性执行时考虑它们。

`Attribute Based` Modifier还可以设置 `SourceTagFilter` 和 `TargetTagFilter`。当确定作为 `Attribute Based` 修饰符来源的属性的大小时，这些过滤器用于排除该属性的某些修饰符。源或目标不具有过滤器所有标签的修饰符将被排除。

具体来说，这意味着：源ASC和目标ASC的标签由`GameplayEffects`捕获。创建 `GameplayEffectSpec` 时会捕获源 ASC 标签，在执行效果时会捕获目标 ASC 标签。在确定无限或持续时间效果的修饰符是否“有资格”应用（即其聚合器有资格）并且设置了这些过滤器时，将捕获的标签与过滤器进行比较。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge-stacking"></a>
#### 4.5.5 Gameplay Effect 的叠加
默认情况下，`GameplayEffects` 将应用 `GameplayEffectSpec` 的新实例，这些实例不知道也不关心应用程序中先前存在的 `GameplayEffectSpec` 实例。 `GameplayEffects` 可以设置为堆栈，其中不添加 `GameplayEffectSpec` 的新实例，而是更改当前现有的 `GameplayEffectSpec's` 堆栈计数。堆叠仅适用于 `Duration` 和 `Infinite` `GameplayEffects`。

有两种类型的堆叠：按源聚合和按目标聚合。

| 堆叠式       | 描述                                                                                                                          |
| ------------------- | ------------------------------------------------------------------------------------------------------------------------------------ |
| 按来源汇总 | 目标上的每个源 `ASC` 有一个单独的堆栈实例。每个源可以应用 X 数量的堆栈。                     |
| 按目标聚合 | 无论源如何，目标上都只有一个堆栈实例。每个源可以应用一个堆栈，最高可达共享堆栈限制。 |

堆栈还具有过期、持续时间刷新和周期重置的策略。他们在 `GameplayEffect` 蓝图中提供了有用的悬停工具提示。

示例项目包括一个自定义蓝图节点，用于侦听 `GameplayEffect` 堆栈更改。 HUD UMG Widget 使用它来更新玩家拥有的被动装甲堆栈数量。这个 `AsyncTask` 将永远存在，直到手动调用 `EndTask()`，我们在 UMG Widget 的 `Destruct` 事件中执行此操作。参见 `AsyncTaskEffectStackChanged.h/cpp`。

![Listen for GameplayEffect Stack Change BP Node](https://github.com/tranek/GASDocumentation/raw/master/Images/gestackchange.png)

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge-ga"></a>
#### 4.5.6 授予的 Abilities
`GameplayEffects`可以将新的[`GameplayAbilities`](#concepts-ga)授予`ASCs`。只有`Duration`和`Infinite` `GameplayEffects`可以授予能力。

一个常见的用例是当你想要强迫其他玩家做一些事情，比如将他们从击退或拉扯中移动时。您可以将 `GameplayEffect` 应用于它们，授予它们自动激活能力（请参阅 [被动 Abilities](#concepts-ga-activating-passive) 了解如何在授予能力时自动激活能力），对它们执行所需的操作。

设计者可以选择 `GameplayEffect` 授予哪些能力、授予它们的级别、[输入绑定](#concepts-ga-input) 的授予能力以及授予能力的删除政策。

| 搬迁政策             | 描述                                                                                                                                                                     |
| -------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 立即取消能力 | 当授予该能力的 `GameplayEffect` 从目标中移除时，授予的能力将立即取消并移除。                                                   |
| 结束时移除能力      | 授予的能力可以完成，然后从目标中删除。                                                                                                   |
| 什么都不做                 | 授予的能力不受从目标中删除授予的 `GameplayEffect` 的影响。目标永久具有该能力，直到稍后手动将其移除。 |

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge-tags"></a>
#### 4.5.7 Gameplay Effect 的 Tags
`GameplayEffects`携带多个[`GameplayTagContainers`](#concepts-gt)。设计者将针对每个类别编辑`Added`和`Removed` `GameplayTagContainers`，结果将在编译时显示在`Combined` `GameplayTagContainer`中。 `Added` 标签是 `GameplayEffect` 添加的新标签，其父级以前没有。 `Removed`标签是父类有但该子类没有的标签。

| 类别                          | 描述                                                                                                                                                                                                                                                                                                                                                                        |
| --------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| GameplayEffect资产标签        | `GameplayEffect` 具有的标签。它们本身不执行任何功能，仅用于描述 `GameplayEffect`。                                                                                                                                                                                                                                        |
| 授予标签                      | 标签位于 `GameplayEffect` 上，但也赋予 `GameplayEffect` 所应用的 `ASC`。当 `GameplayEffect` 被移除时，它们也会从 `ASC` 中移除。这仅适用于 `Duration` 和 `Infinite` `GameplayEffects`。                                                                                                                             |
| 持续的标签要求          | 应用后，这些标签将确定 `GameplayEffect` 是打开还是关闭。 `GameplayEffect` 可以关闭但仍可应用。如果 `GameplayEffect` 由于未满足持续标签要求而关闭，但随后满足要求，则 `GameplayEffect` 将再次打开并重新应用其Modifier。这仅适用于 `Duration` 和 `Infinite` `GameplayEffects`。 |
| 应用程序标签要求      | 目标上的标签确定 `GameplayEffect` 是否可以应用于目标。如果不满足这些要求，则不适用 `GameplayEffect`。                                                                                                                                                                                                                      |
| 删除带有标签的游戏​​效果 | `GameplayEffects`在目标上有任何这些标签`Asset Tags`或者`Granted Tags`将从目标中删除时`GameplayEffect`已成功应用。                                                                                                                                                                                            |

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge-immunity"></a>
#### 4.5.8 免疫
`GameplayEffects`可以授予免疫力，有效阻止其他`GameplayEffects`的应用，基于[`GameplayTags`](#concepts-gt)。虽然可以通过 `Application Tag Requirements` 等其他方式有效地实现免疫，但使用此系统可以在 `GameplayEffects` 由于免疫 `UAbilitySystemComponent::OnImmunityBlockGameplayEffectDelegate` 而被阻止时提供代表。

`GrantedApplicationImmunityTags` 检查源 `ASC`（包括来自源能力 `AbilityTags` 的标签，如果有的话）是否具有任何指定标签。这是一种根据标签的某些字符或来源提供对所有 `GameplayEffects` 的免疫力的方法。

`Granted Application Immunity Query` 检查传入的 `GameplayEffectSpec` 是否与任何查询匹配，以阻止或允许其应用程序。

这些查询在 `GameplayEffect` 蓝图中具有有用的悬停工具提示。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge-spec"></a>
#### 4.5.9 Gameplay Effect Spec
[`GameplayEffectSpec`](https://docs.unrealengine.com/en-US/API/Plugins/GameplayAbilities/FGameplayEffectSpec/index.html)（`GESpec`）可以理解为实例化后的 `GameplayEffects`。它保存所代表的 `GameplayEffect` 类的引用、创建时的等级以及创建者信息。与通常由设计人员在运行前创建的 `GameplayEffects` 不同，Spec 可以在运行时、应用之前自由创建和修改。应用 `GameplayEffect` 时，系统会根据它创建 `GameplayEffectSpec`，实际应用到目标上的是这个 Spec。

`GameplayEffectSpecs` 是使用 `UAbilitySystemComponent::MakeOutgoingSpec()` 从 `GameplayEffects` 创建的，即 `BlueprintCallable`。 `GameplayEffectSpecs` 不必立即应用。通常将 `GameplayEffectSpec` 传递给由弹丸可以应用于其稍后击中的目标的能力创建的弹丸。当 `GameplayEffectSpecs` 成功应用时，它们返回一个名为 `FActiveGameplayEffect` 的新结构。

值得注意的 `GameplayEffectSpec` 内容：
* 创建此 `GameplayEffect` 的 `GameplayEffect` 类。
* 这`GameplayEffectSpec`的水平。通常与创建 `GameplayEffectSpec` 的能力级别相同，但也可以不同。
* `GameplayEffectSpec` 的持续时间。默认为 `GameplayEffect` 的持续时间，但可以不同。
* 用于周期性效应的 `GameplayEffectSpec` 的周期。默认为 `GameplayEffect` 的周期，但可以不同。
* 该 `GameplayEffectSpec` 的当前堆栈计数。堆栈限制位于 `GameplayEffect` 上。
* [`GameplayEffectContextHandle`](#concepts-ge-context) 告诉我们是谁创建了这个 `GameplayEffectSpec`。
* `Attributes` 在创建 `GameplayEffectSpec` 时由于快照而捕获。
* 除`GameplayEffect`授予的`GameplayTags`外，`GameplayEffectSpec`向目标公司授予的`DynamicGrantedTags`。
* 除了 `GameplayEffect` 具有的 `AssetTags` 之外，`GameplayEffectSpec` 还具有 `DynamicAssetTags`。
* `SetByCaller` `TMaps`.

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge-spec-setbycaller"></a>
##### 4.5.9.1 SetByCallers
`SetByCallers` 允许 `GameplayEffectSpec` 携带与 `GameplayTag` 或 `FName` 相关的float 值。它们存储在 `GameplayEffectSpec` 上各自的 `TMaps` 中：`TMap<FGameplayTag, float>` 和 `TMap<FName, float>`。这些可以用作 `GameplayEffect` 上的 `Modifiers` 或作为运送浮体的通用方法。通常通过 `SetByCallers` 将能力内部生成的数值数据传递到 [`GameplayEffectExecutionCalculations`](#concepts-ge-ec) 或 [`ModifierMagnitudeCalculations`](#concepts-ge-mmc)。

| `SetByCaller` 使用 | 笔记                                                                                                                                                                                                                                                                                                                                                                                                                               |
| ----------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `Modifiers`       | 必须在 `GameplayEffect` 类中提前定义。只能使用`GameplayTag`版本。如果在 `GameplayEffect` 类上定义了一个，但 `GameplayEffectSpec` 没有相应的标记和float 值对，则游戏在应用 `GameplayEffectSpec` 时会出现运行时错误并返回 0。这是 `Divide` 操作的潜在问题。参见 [`Modifiers`](#concepts-ge-mods)。 |
| 别处         | 不需要在任何地方提前定义。读取 `GameplayEffectSpec` 上不存在的 `SetByCaller` 可以返回开发人员定义的默认值，并带有可选警告。                                                                                                                                                                                                                                      |

要在蓝图中分配 `SetByCaller` 值，请使用您需要的版本的蓝图节点（`GameplayTag` 或 `FName`）：

![Assigning SetByCaller](https://github.com/tranek/GASDocumentation/raw/master/Images/setbycaller.png)

要读取蓝图中的 `SetByCaller` 值，您需要在蓝图库中创建自定义节点。

要在 C++ 中分配 `SetByCaller` 值，请使用您需要的函数版本（`GameplayTag` 或 `FName`）：

```c++
void FGameplayEffectSpec::SetSetByCallerMagnitude(FName DataName, float Magnitude);
```
```c++
void FGameplayEffectSpec::SetSetByCallerMagnitude(FGameplayTag DataTag, float Magnitude);
```

要读取 C++ 中的 `SetByCaller` 值，请使用您需要的函数版本（`GameplayTag` 或 `FName`）：

```c++
float GetSetByCallerMagnitude(FName DataName, bool WarnIfNotFound = true, float DefaultIfNotFound = 0.f) const;
```
```c++
float GetSetByCallerMagnitude(FGameplayTag DataTag, bool WarnIfNotFound = true, float DefaultIfNotFound = 0.f) const;
```

我建议使用 `GameplayTag` 版本而不是 `FName` 版本。这可以防止蓝图中的拼写错误。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge-context"></a>
#### 4.5.10 Gameplay Effect Context
[`GameplayEffectContext`](https://docs.unrealengine.com/en-US/API/Plugins/GameplayAbilities/FGameplayEffectContext/index.html) 结构保存有关 `GameplayEffectSpec's` 煽动者和 [`TargetData`](#concepts-targeting-data) 的信息。这也是一个很好的子类结构，可以在 [`ModifierMagnitudeCalculations`](#concepts-ge-mmc) / [`GameplayEffectExecutionCalculations`](#concepts-ge-ec)、[`AttributeSets`](#concepts-as) 和 [`GameplayCues`](#concepts-gc) 等位置之间传递任意数据。

要子类化 `GameplayEffectContext`：

1. 子类 `FGameplayEffectContext`
1. 覆盖`FGameplayEffectContext::GetScriptStruct()`
1. 覆盖`FGameplayEffectContext::Duplicate()`
1. 如果需要复制新数据，请覆盖 `FGameplayEffectContext::NetSerialize()`
1. 为您的子类实现 `TStructOpsTypeTraits`，就像父结构 `FGameplayEffectContext` 一样
1. 重写 [`AbilitySystemGlobals`](#concepts-asg) 类中的 `AllocGameplayEffectContext()` 以返回子类的新对象

[GASShooter](https://github.com/tranek/GASShooter)使用`GameplayEffectContext`的子类来添加`TargetData`，可以在`GameplayCues`中访问它，专门用于霰弹枪，因为它可以击中多个敌人。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge-mmc"></a>
#### 4.5.11 Modifier Magnitude Calculation（MMC）
[`ModifierMagnitudeCalculations`](https://docs.unrealengine.com/en-US/API/Plugins/GameplayAbilities/UGameplayModMagnitudeCalculation/index.html)（`ModMagCalc` 或 `MMC`）是功能强大的类，在 `GameplayEffects` 中用作 [`Modifiers`](#concepts-ge-mods)。它们的功能与 [`GameplayEffectExecutionCalculations`](#concepts-ge-ec) 类似，但功能较弱，最重要的是它们可以是 [predicted](#concepts-p)。它们的唯一目的是从 `CalculateBaseMagnitude_Implementation()` 返回float 值。您可以在蓝图和 C++ 中子类化并重写此函数。

`MMCs` 可在 `GameplayEffects` - `Instant`、`Duration`、`Infinite` 或 `Periodic` 的任何持续时间内使用。

`MMCs'`的优势在于它们能够捕获`Source`上任意数量的`Attributes`或`GameplayEffect`的`Target`的值，并完全访问`GameplayEffectSpec`以读取`GameplayTags`和`SetByCallers`。 `Attributes` 可以快照也可以不快照。创建 `GameplayEffectSpec` 时捕获快照 `Attributes`，而应用 `GameplayEffectSpec` 时捕获非快照 `Attributes`，并在 `Infinite` 和 `Duration` `GameplayEffects` 的 `Attribute` 更改时自动更新。捕获 `Attributes` 会根据 `ASC` 上的现有 mod 重新计算其 `CurrentValue`。此重新计算将**不会**在 `AbilitySet` 中运行 [`PreAttributeChange()`](#concepts-as-preattributechange)，因此必须在此处再次进行任何钳位。

| 快照 | 源或目标 | 拍摄于 `GameplayEffectSpec` | 当 `Attribute` 更改为 `Infinite` 或 `Duration` `GE` 时自动更新 |
| -------- | ---------------- | -------------------------------- | -------------------------------------------------------------------------------- |
| 是的      | 来源           | 创建                         | 不                                                                               |
| 是的      | 目标           | 应用                      | 不                                                                               |
| 不       | 来源           | 应用                      | 是的                                                                              |
| 不       | 目标           | 应用                      | 是的                                                                              |

`MMC` 生成的float可以在 `GameplayEffect's` `Modifier` 中通过系数以及前后系数相加进一步修改。

捕获 `Target's` 法力 `Attribute` 的示例 `MMC` 会减少其中毒效果，其中减少的量根据 `Target` 拥有的法力数量以及 `Target` 可能拥有的标签而变化：
```c++
UPAMMC_PoisonMana::UPAMMC_PoisonMana()
{

	//ManaDef defined in header FGameplayEffectAttributeCaptureDefinition ManaDef;
	ManaDef.AttributeToCapture = UPAAttributeSetBase::GetManaAttribute();
	ManaDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	ManaDef.bSnapshot = false;

	//MaxManaDef defined in header FGameplayEffectAttributeCaptureDefinition MaxManaDef;
	MaxManaDef.AttributeToCapture = UPAAttributeSetBase::GetMaxManaAttribute();
	MaxManaDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	MaxManaDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(ManaDef);
	RelevantAttributesToCapture.Add(MaxManaDef);
}

float UPAMMC_PoisonMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec & Spec) const
{
	// Gather the tags from the source and target as that can affect which buffs should be used
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Mana = 0.f;
	GetCapturedAttributeMagnitude(ManaDef, Spec, EvaluationParameters, Mana);
	Mana = FMath::Max<float>(Mana, 0.0f);

	float MaxMana = 0.f;
	GetCapturedAttributeMagnitude(MaxManaDef, Spec, EvaluationParameters, MaxMana);
	MaxMana = FMath::Max<float>(MaxMana, 1.0f); // Avoid divide by zero

	float Reduction = -20.0f;
	if (Mana / MaxMana > 0.5f)
	{
		// Double the effect if the target has more than half their mana
		Reduction *= 2;
	}
	
	if (TargetTags->HasTagExact(FGameplayTag::RequestGameplayTag(FName("Status.WeakToPoisonMana"))))
	{
		// Double the effect if the target is weak to PoisonMana
		Reduction *= 2;
	}
	
	return Reduction;
}
```

如果您不在 `MMC's` 构造函数中将 `FGameplayEffectAttributeCaptureDefinition` 添加到 `RelevantAttributesToCapture` 并尝试捕获 `Attributes`，则捕获时会收到有关缺少 Spec 的错误。如果您不需要捕获 `Attributes`，则无需向 `RelevantAttributesToCapture` 添加任何内容。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge-ec"></a>
#### 4.5.12 Gameplay Effect Execution Calculation（ExecCalc）
[`GameplayEffectExecutionCalculations`](https://docs.unrealengine.com/en-US/API/Plugins/GameplayAbilities/UGameplayEffectExecutionCalculat-/index.html)（`ExecutionCalculation`、`Execution`（您经常会在插件的源代码中看到这个术语）或 `ExecCalc`）是 `GameplayEffects` 对 `ASC` 进行更改的最强大方法。与 [`ModifierMagnitudeCalculations`](#concepts-ge-mmc) 一样，它们可以捕获 `Attributes` 并可选择对其进行快照。与 `MMCs` 不同，这些可以更改多个 `Attribute`，并且基本上可以执行程序员想要的任何其他操作。这种功能和灵活性的缺点是它们不能是 [predicted](#concepts-p) 并且必须用 C++ 实现。

`ExecutionCalculations` 只能与 `Instant` 和 `Periodic` `GameplayEffects` 一起使用。任何带有“执行”一词的内容通常指的是这两种类型的 `GameplayEffects`。

创建 `GameplayEffectSpec` 时，快照会捕获 `Attribute`，而应用 `GameplayEffectSpec` 时，不快照会捕获 `Attribute`。捕获 `Attributes` 会根据 `ASC` 上的现有 mod 重新计算其 `CurrentValue`。此重新计算将**不会**在 `AbilitySet` 中运行 [`PreAttributeChange()`](#concepts-as-preattributechange)，因此必须在此处再次进行任何钳位。

| 快照 | 源或目标 | 拍摄于 `GameplayEffectSpec` |
| -------- | ---------------- | -------------------------------- |
| 是的      | 来源           | 创建                         |
| 是的      | 目标           | 应用                      |
| 不       | 来源           | 应用                      |
| 不       | 目标           | 应用                      |

为了设置 `Attribute` 捕获，我们遵循 Epic 的 ActionRPG 示例项目设置的模式，定义一个结构体来保存并定义我们如何捕获 `Attributes` 并在该结构体的构造函数中创建它的一个副本。对于每个 `ExecCalc`，您都会有一个这样的结构。 **注意：** 每个结构体都需要一个唯一的名称，因为它们共享相同的命名空间。对结构使用相同的名称将导致捕获 `Attributes` 时出现错误行为（主要是捕获错误的 `Attributes` 的值）。

对于`Local Predicted`、`Server Only`、`Server Initiated`、[`GameplayAbilities`](#concepts-ga)，`ExecCalc`仅调用服务器。

根据读取 `Source` 和 `Target` 上许多属性的复杂公式计算受到的伤害是 `ExecCalc` 最常见的示例。包含的示例项目有一个简单的 `ExecCalc` 用于计算伤害，它从 `GameplayEffectSpec's` [`SetByCaller`](#concepts-ge-spec-setbycaller) 读取伤害值，然后根据从 `Target` 捕获的装甲 `Attribute` 减轻该值。参见 `GDDamageExecCalculation.cpp/.h`。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge-ec-senddata"></a>
##### 4.5.12.1 向 Execution Calculation 传递数据
除了捕获 `Attributes` 之外，还有几种方法可以将数据发送到 `ExecutionCalculation`。

<a name="concepts-ge-ec-senddata-setbycaller"></a>
###### 4.5.12.1.1 SetByCaller
任何[`SetByCallers` set on the `GameplayEffectSpec`](#concepts-ge-spec-setbycaller)都可以在`ExecutionCalculation`中直接读取。

```c++
const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
float Damage = FMath::Max<float>(Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, -1.0f), 0.0f);
```

<a name="concepts-ge-ec-senddata-backingdataattribute"></a>
###### 4.5.12.1.2 使用 Attribute 作为数据来源的 Calculation Modifier
如果您想将值硬编码为`GameplayEffect`，你可以使用它们传递`CalculationModifier`使用捕获的之一`Attributes`作为支持数据。

在此屏幕截图示例中，我们将 50 添加到捕获的伤害 `Attribute` 中。您还可以将其设置为 `Override` 以仅接受硬编码值。

![Backing Data Attribute Calculation Modifier](https://github.com/tranek/GASDocumentation/raw/master/Images/calculationmodifierbackingdataattribute.png)

`ExecutionCalculation` 在捕获 `Attribute` 时读取该值。

```c++
float Damage = 0.0f;
// Capture optional damage value set on the damage GE as a CalculationModifier under the ExecutionCalculation
ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DamageDef, EvaluationParameters, Damage);
```

<a name="concepts-ge-ec-senddata-backingdatatempvariable"></a>
###### 4.5.12.1.3 使用临时变量作为数据来源的 Calculation Modifier
如果想将数值硬编码在 `GameplayEffect` 中，可以通过使用 `Temporary Variable` 的 `CalculationModifier` 传递这些数值。它在 C++ 中也称为 `Transient Aggregator`。每个 `Temporary Variable` 都与一个 `GameplayTag` 关联。

在此屏幕截图示例中，我们使用 `Data.Damage` `GameplayTag` 将 50 添加到 `Temporary Variable`。

![Backing Data Temporary Variable Calculation Modifier](https://github.com/tranek/GASDocumentation/raw/master/Images/calculationmodifierbackingdatatempvariable.png)

将支持 `Temporary Variables` 添加到 `ExecutionCalculation` 的构造函数中：

```c++
ValidTransientAggregatorIdentifiers.AddTag(FGameplayTag::RequestGameplayTag("Data.Damage"));
```

`ExecutionCalculation` 使用与 `Attribute` 捕获功能类似的特殊捕获功能来读取该值。

```c++
float Damage = 0.0f;
ExecutionParams.AttemptCalculateTransientAggregatorMagnitude(FGameplayTag::RequestGameplayTag("Data.Damage"), EvaluationParameters, Damage);
```

<a name="concepts-ge-ec-senddata-effectcontext"></a>
###### 4.5.12.1.4 Gameplay Effect Context
您可以通过自定义 [`GameplayEffectContext` on the `GameplayEffectSpec`](#concepts-ge-context) 向 `ExecutionCalculation` 发送数据。

在`ExecutionCalculation`中，您可以从`FGameplayEffectCustomExecutionParameters`访问`EffectContext`。

```c++
const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
FGSGameplayEffectContext* ContextHandle = static_cast<FGSGameplayEffectContext*>(Spec.GetContext().Get());
```

如果您需要更改 `GameplayEffectSpec` 或 `EffectContext` 上的某些内容：

```c++
FGameplayEffectSpec* MutableSpec = ExecutionParams.GetOwningSpecForPreExecuteMod();
FGSGameplayEffectContext* ContextHandle = static_cast<FGSGameplayEffectContext*>(MutableSpec->GetContext().Get());
```

修改`ExecutionCalculation`中的`GameplayEffectSpec`时请小心。请参阅 `GetOwningSpecForPreExecuteMod()` 的评论。

```c++
/** Non const access. Be careful with this, especially when modifying a spec after attribute capture. */
FGameplayEffectSpec* GetOwningSpecForPreExecuteMod() const;
```

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge-car"></a>
#### 4.5.13 自定义应用条件（Custom Application Requirement）
[`CustomApplicationRequirement`](https://docs.unrealengine.com/en-US/API/Plugins/GameplayAbilities/UGameplayEffectCustomApplication-/index.html) (`CAR`) 类为设计人员提供了对是否可以应用 `GameplayEffect` 的高级控制，而不是对 `GameplayEffect` 进行简单的 `GameplayTag` 检查。这些可以通过覆盖 `CanApplyGameplayEffect()` 在蓝图中实现，也可以通过覆盖 `CanApplyGameplayEffect_Implementation()` 在 C++ 中实现。

何时使用 `CARs` 的示例：
* `Target`需要有一定数量的`Attribute`
* `Target`需要有一定数量的`GameplayEffect`堆叠

`CARs` 还可以执行更高级的操作，例如检查此 `GameplayEffect` 的实例是否已在现有实例的 `Target` 和 [改变持续时间](#concepts-ge-duration) 上，而不是应用新实例（对于 `CanApplyGameplayEffect()` 返回 false）。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge-cost"></a>
#### 4.5.14 Cost Gameplay Effect
[`GameplayAbilities`](#concepts-ga) 有一个可选的 `GameplayEffect` 专门设计用作成本能力。成本是 `ASC` 需要多少 `Attribute` 才能激活 `GameplayAbility`。如果 `GA` 买不起 `Cost GE`，那么它们将无法激活。这个`Cost GE`应该是一个`Instant` `GameplayEffect`，其中一个或多个`Modifiers`从`Attributes`中减去。默认情况下，`Cost GEs` 用于预测，建议保持该功能，这意味着不要使用 `ExecutionCalculations`。 `MMCs` 完全可以接受并鼓励进行复杂的成本计算。

开始时，您很可能会为每个 `GA` 拥有一个唯一的 `Cost GE`，并且需要付费。更先进的技术是将一个 `Cost GE` 重复用于多个 `GAs`，并仅使用 `GA` 特定数据修改从 `Cost GE` 创建的 `GameplayEffectSpec`（成本值在 `GA` 上定义）。 **这仅适用于 `Instanced` 能力。**

重用 `Cost GE` 的两种技巧：

1. **使用 `MMC`。** 这是最简单的方法。创建一个 [`MMC`](#concepts-ge-mmc)，从 `GameplayAbility` 实例读取成本值，您可以从 `GameplayEffectSpec` 获取该成本值。

```c++
float UPGMMC_HeroAbilityCost::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec & Spec) const
{
	const UPGGameplayAbility* Ability = Cast<UPGGameplayAbility>(Spec.GetContext().GetAbilityInstance_NotReplicated());

	if (!Ability)
	{
		return 0.0f;
	}

	return Ability->Cost.GetValueAtLevel(Ability->GetAbilityLevel());
}
```

在此示例中，成本值是我添加到其中的 `GameplayAbility` 子类上的 `FScalableFloat`。
```c++
UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cost")
FScalableFloat Cost;
```

![Cost GE With MMC](https://github.com/tranek/GASDocumentation/raw/master/Images/costmmc.png)

2. **覆盖 `UGameplayAbility::GetCostGameplayEffect()`。** 覆盖此函数和读取 `GameplayAbility` 上成本值的 [create a `GameplayEffect` at runtime](#concepts-ge-dynamic)。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge-cooldown"></a>
#### 4.5.15 Cooldown Gameplay Effect
[`GameplayAbilities`](#concepts-ga) 有一个可选的 `GameplayEffect` 专门设计用作能力的冷却时间。冷却时间决定了激活后多长时间可以再次激活该能力。如果 `GA` 仍处于冷却状态，则无法激活。此 `Cooldown GE` 应该是 `Duration` `GameplayEffect`，没有 `Modifiers`，并且 `GameplayEffect's` `GrantedTags` 中的每个 `GameplayAbility` 或每个能力槽位（如果您的游戏具有分配给共享冷却时间的槽位的可互换能力）一个唯一的 `GameplayTag` （“`Cooldown Tag`”）。 `GA` 实际上检查 `Cooldown Tag` 是否存在，而不是 `Cooldown GE` 是否存在。默认情况下，`Cooldown GEs` 用于预测，建议保持该功能，即不使用 `ExecutionCalculations`。 `MMCs` 完全可以接受并鼓励复杂的冷却计算。

开始时，您很可能会为每个 `GA` 拥有一个具有冷却时间的独特 `Cooldown GE`。更先进的技术是将一个 `Cooldown GE` 重复用于多个 `GAs`，并仅使用 `GA` 特定数据修改从 `Cooldown GE` 创建的 `GameplayEffectSpec`（冷却时间和 `Cooldown Tag` 在 `GA` 上定义）。 **这仅适用于 `Instanced` 能力。**

重用 `Cooldown GE` 的两种技巧：

1. **使用 [`SetByCaller`](#concepts-ge-spec-setbycaller)。** 这是最简单的方法。使用 `GameplayTag` 将共享 `Cooldown GE` 的持续时间设置为 `SetByCaller`。在您的 `GameplayAbility` 子类上，为持续时间定义一个 float / `FScalableFloat`，为唯一的 `Cooldown Tag` 定义一个 `FGameplayTagContainer`，以及一个临时 `FGameplayTagContainer`，我们将使用它作为 `Cooldown Tag` 和 `Cooldown GE's` 标记的并集的返回指针。
```c++
UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown")
FScalableFloat CooldownDuration;

UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown")
FGameplayTagContainer CooldownTags;

// Temp container that we will return the pointer to in GetCooldownTags().
// This will be a union of our CooldownTags and the Cooldown GE's cooldown tags.
UPROPERTY(Transient)
FGameplayTagContainer TempCooldownTags;
```

然后覆盖 `UGameplayAbility::GetCooldownTags()` 以返回 `Cooldown Tags` 和任何现有 `Cooldown GE's` 标签的并集。
```c++
const FGameplayTagContainer * UPGGameplayAbility::GetCooldownTags() const
{
	FGameplayTagContainer* MutableTags = const_cast<FGameplayTagContainer*>(&TempCooldownTags);
	MutableTags->Reset(); // MutableTags writes to the TempCooldownTags on the CDO so clear it in case the ability cooldown tags change (moved to a different slot)
	const FGameplayTagContainer* ParentTags = Super::GetCooldownTags();
	if (ParentTags)
	{
		MutableTags->AppendTags(*ParentTags);
	}
	MutableTags->AppendTags(CooldownTags);
	return MutableTags;
}
```

最后，覆盖 `UGameplayAbility::ApplyCooldown()` 以注入我们的 `Cooldown Tags` 并将 `SetByCaller` 添加到冷却时间 `GameplayEffectSpec` 中。
```c++
void UPGGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (CooldownGE)
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
		SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(CooldownTags);
		SpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName(  OurSetByCallerTag  )), CooldownDuration.GetValueAtLevel(GetAbilityLevel()));
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
}
```

在此图中，冷却时间 `Modifier` 设置为 `SetByCaller`，`Data Tag` 设置为 `Data.Cooldown`。在上面的代码中，`Data.Cooldown` 将是 `OurSetByCallerTag`。

![Cooldown GE with SetByCaller](https://github.com/tranek/GASDocumentation/raw/master/Images/cooldownsbc.png)

2. **使用 [`MMC`](#concepts-ge-mmc)。** 除了将 `SetByCaller` 设置为 `Cooldown GE` 和 `ApplyCooldown` 上的持续时间之外，其设置与上述相同。相反，将持续时间设置为 `Custom Calculation Class` 并指向我们将创建的新 `MMC`。
```c++
UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown")
FScalableFloat CooldownDuration;

UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Cooldown")
FGameplayTagContainer CooldownTags;

// Temp container that we will return the pointer to in GetCooldownTags().
// This will be a union of our CooldownTags and the Cooldown GE's cooldown tags.
UPROPERTY(Transient)
FGameplayTagContainer TempCooldownTags;
```

然后覆盖 `UGameplayAbility::GetCooldownTags()` 以返回 `Cooldown Tags` 和任何现有 `Cooldown GE's` 标签的并集。
```c++
const FGameplayTagContainer * UPGGameplayAbility::GetCooldownTags() const
{
	FGameplayTagContainer* MutableTags = const_cast<FGameplayTagContainer*>(&TempCooldownTags);
	MutableTags->Reset(); // MutableTags writes to the TempCooldownTags on the CDO so clear it in case the ability cooldown tags change (moved to a different slot)
	const FGameplayTagContainer* ParentTags = Super::GetCooldownTags();
	if (ParentTags)
	{
		MutableTags->AppendTags(*ParentTags);
	}
	MutableTags->AppendTags(CooldownTags);
	return MutableTags;
}
```

最后，覆盖 `UGameplayAbility::ApplyCooldown()` 将我们的 `Cooldown Tags` 注入冷却 `GameplayEffectSpec` 中。
```c++
void UPGGameplayAbility::ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (CooldownGE)
	{
		FGameplayEffectSpecHandle SpecHandle = MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
		SpecHandle.Data.Get()->DynamicGrantedTags.AppendTags(CooldownTags);
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, SpecHandle);
	}
}
```

```c++
float UPGMMC_HeroAbilityCooldown::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec & Spec) const
{
	const UPGGameplayAbility* Ability = Cast<UPGGameplayAbility>(Spec.GetContext().GetAbilityInstance_NotReplicated());

	if (!Ability)
	{
		return 0.0f;
	}

	return Ability->CooldownDuration.GetValueAtLevel(Ability->GetAbilityLevel());
}
```

![Cooldown GE with MMC](https://github.com/tranek/GASDocumentation/raw/master/Images/cooldownmmc.png)

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge-cooldown-tr"></a>
##### 4.5.15.1 获取 Cooldown Gameplay Effect 的剩余时间
```c++
bool APGPlayerState::GetCooldownRemainingForTag(FGameplayTagContainer CooldownTags, float & TimeRemaining, float & CooldownDuration)
{
	if (AbilitySystemComponent && CooldownTags.Num() > 0)
	{
		TimeRemaining = 0.f;
		CooldownDuration = 0.f;

		FGameplayEffectQuery const Query = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTags);
		TArray< TPair<float, float> > DurationAndTimeRemaining = AbilitySystemComponent->GetActiveEffectsTimeRemainingAndDuration(Query);
		if (DurationAndTimeRemaining.Num() > 0)
		{
			int32 BestIdx = 0;
			float LongestTime = DurationAndTimeRemaining[0].Key;
			for (int32 Idx = 1; Idx < DurationAndTimeRemaining.Num(); ++Idx)
			{
				if (DurationAndTimeRemaining[Idx].Key > LongestTime)
				{
					LongestTime = DurationAndTimeRemaining[Idx].Key;
					BestIdx = Idx;
				}
			}

			TimeRemaining = DurationAndTimeRemaining[BestIdx].Key;
			CooldownDuration = DurationAndTimeRemaining[BestIdx].Value;

			return true;
		}
	}

	return false;
}
```

**注意：** 查询客户端的剩余冷却时间需要客户端能够收到复制的`GameplayEffects`。这将取决于他们的 `ASC's` [复制模式](#concepts-asc-rm)。

<a name="concepts-ge-cooldown-listen"></a>
##### 4.5.15.2 监听冷却开始与结束
要侦听冷却时间何时开始，您可以通过绑定到 `AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf` 来响应何时应用 `Cooldown GE`，或者通过绑定到 `AbilitySystemComponent->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved)` 来响应何时添加 `Cooldown Tag`。我建议监听何时添加 `Cooldown GE`，因为您还可以访问应用它的 `GameplayEffectSpec`。由此您可以确定 `Cooldown GE` 是本地预测的还是服务器校正的。

监听冷却结束有两种方式：绑定 `AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate()`，响应 `Cooldown GE` 被移除；或者绑定 `AbilitySystemComponent->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved)`，响应 `Cooldown Tag` 被移除。我推荐监听 `Cooldown Tag` 的移除，因为服务器修正后的 `Cooldown GE` 到达时，会移除本地预测的 GE，即使仍处于冷却中也会触发 `OnAnyGameplayEffectRemovedDelegate()`。在移除预测的 `Cooldown GE` 并应用服务器修正后的 `Cooldown GE` 这一过程中，`Cooldown Tag` 不会改变。

**注意：** 在客户端上侦听要添加或删除的 `GameplayEffect` 要求它们可以接收复制的 `GameplayEffects`。这将取决于他们的 `ASC's` [复制模式](#concepts-asc-rm)。

示例项目包括一个自定义蓝图节点，用于侦听冷却时间的开始和结束。 HUD UMG Widget 使用它来更新流星冷却的剩余时间。这个 `AsyncTask` 将永远存在，直到手动调用 `EndTask()`，我们在 UMG Widget 的 `Destruct` 事件中执行此操作。参见 [`AsyncTaskCooldownChanged.h/cpp`](Source/GASDocumentation/Private/Characters/Abilities/AsyncTaskCooldownChanged.cpp)。

![Listen for Cooldown Change BP Node](https://github.com/tranek/GASDocumentation/raw/master/Images/cooldownchange.png)

<a name="concepts-ge-cooldown-prediction"></a>
##### 4.5.15.3 预测冷却
目前无法真正预测冷却时间。当应用本地预测的 `Cooldown GE` 时，我们可以启动 UI 冷却计时器，但 `GameplayAbility's` 实际冷却时间与服务器的剩余冷却时间相关。根据玩家的延迟，本地预测的冷却时间可能会到期，但 `GameplayAbility` 仍将在服务器上处于冷却状态，这将阻止 `GameplayAbility's` 立即重新激活，直到服务器的冷却时间到期。

示例项目通过在本地预测的冷却时间开始时使流星能力的 UI 图标变灰，然后在服务器校正的 `Cooldown GE` 进入后启动冷却计时器来处理此问题。

这样做的一个游戏结果是，高延迟的玩家在短冷却能力上的射速比低延迟的玩家低，这使他们处于劣势。 Fortnite 通过他们的武器具有不使用冷却时间 `GameplayEffects` 的自定义簿记来避免这种情况。

Epic 希望有一天在 [GAS 的未来迭代](#concepts-p-future) 中实现真正的预测冷却时间（当本地冷却时间到期但服务器仍处于冷却状态时，玩家可以激活 `GameplayAbility`）。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge-duration"></a>
#### 4.5.16 修改生效中的 Gameplay Effect 的持续时间
要更改 `Cooldown GE` 或任何 `Duration` `GameplayEffect` 的剩余时间，我们需要更改 `GameplayEffectSpec's` `Duration`，更新其 `StartServerWorldTime`，更新其 `CachedStartServerWorldTime`，更新其 `StartWorldTime`，然后使用以下命令重新运行持续时间检查： `CheckDuration()`。在服务器上执行此操作并将 `FActiveGameplayEffect` 标记为脏会将更改复制到客户端。
**注意：** 这确实涉及 `const_cast`，并且可能不是 Epic 改变持续时间的预期方式，但到目前为止似乎效果很好。

```c++
bool UPAAbilitySystemComponent::SetGameplayEffectDurationHandle(FActiveGameplayEffectHandle Handle, float NewDuration)
{
	if (!Handle.IsValid())
	{
		return false;
	}

	const FActiveGameplayEffect* ActiveGameplayEffect = GetActiveGameplayEffect(Handle);
	if (!ActiveGameplayEffect)
	{
		return false;
	}

	FActiveGameplayEffect* AGE = const_cast<FActiveGameplayEffect*>(ActiveGameplayEffect);
	if (NewDuration > 0)
	{
		AGE->Spec.Duration = NewDuration;
	}
	else
	{
		AGE->Spec.Duration = 0.01f;
	}

	AGE->StartServerWorldTime = ActiveGameplayEffects.GetServerWorldTime();
	AGE->CachedStartServerWorldTime = AGE->StartServerWorldTime;
	AGE->StartWorldTime = ActiveGameplayEffects.GetWorldTime();
	ActiveGameplayEffects.MarkItemDirty(*AGE);
	ActiveGameplayEffects.CheckDuration(Handle);

	AGE->EventSet.OnTimeChanged.Broadcast(AGE->Handle, AGE->StartWorldTime, AGE->GetDuration());
	OnGameplayEffectDurationChange(*AGE);

	return true;
}
```

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge-dynamic"></a>
#### 4.5.17 在运行时动态创建 Gameplay Effects
在运行时创建动态 `GameplayEffects` 是一个高级主题。您不必经常这样做。

在 C++ 中，只有 `Instant` `GameplayEffects` 可以在运行时从头开始创建。 `Duration` 和 `Infinite` `GameplayEffects` 无法在运行时动态创建，因为它们在复制时会查找不存在的 `GameplayEffect` 类定义。要实现此功能，您应该像通常在编辑器中那样创建原型 `GameplayEffect` 类。然后根据运行时的需要自定义 `GameplayEffectSpec` 实例。

在运行时创建的 `Instant` `GameplayEffects` 也可以从 [局部预测](#concepts-p) `GameplayAbility` 内部调用。然而，目前尚不清楚动态创建是否会产生副作用。

##### 示例

示例项目创建了一个当角色的 `AttributeSet` 受到致命一击时将金币和经验值发送回给角色的杀手。

```c++
// Create a dynamic instant Gameplay Effect to give the bounties
UGameplayEffect* GEBounty = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("Bounty")));
GEBounty->DurationPolicy = EGameplayEffectDurationType::Instant;

int32 Idx = GEBounty->Modifiers.Num();
GEBounty->Modifiers.SetNum(Idx + 2);

FGameplayModifierInfo& InfoXP = GEBounty->Modifiers[Idx];
InfoXP.ModifierMagnitude = FScalableFloat(GetXPBounty());
InfoXP.ModifierOp = EGameplayModOp::Additive;
InfoXP.Attribute = UGDAttributeSetBase::GetXPAttribute();

FGameplayModifierInfo& InfoGold = GEBounty->Modifiers[Idx + 1];
InfoGold.ModifierMagnitude = FScalableFloat(GetGoldBounty());
InfoGold.ModifierOp = EGameplayModOp::Additive;
InfoGold.Attribute = UGDAttributeSetBase::GetGoldAttribute();

Source->ApplyGameplayEffectToSelf(GEBounty, 1.0f, Source->MakeEffectContext());
```

第二个示例显示了在本地预测的 `GameplayAbility` 中创建的运行时 `GameplayEffect`。使用风险自负（请参阅代码中的注释）！

```c++
UGameplayAbilityRuntimeGE::UGameplayAbilityRuntimeGE()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}

void UGameplayAbilityRuntimeGE::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (HasAuthorityOrPredictionKey(ActorInfo, &ActivationInfo))
	{
		if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		{
			EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		}

		// Create the GE at runtime.
		UGameplayEffect* GameplayEffect = NewObject<UGameplayEffect>(GetTransientPackage(), TEXT("RuntimeInstantGE"));
		GameplayEffect->DurationPolicy = EGameplayEffectDurationType::Instant; // Only instant works with runtime GE.

		// Add a simple scalable float modifier, which overrides MyAttribute with 42.
		// In real world applications, consume information passed via TriggerEventData.
		const int32 Idx = GameplayEffect->Modifiers.Num();
		GameplayEffect->Modifiers.SetNum(Idx + 1);
		FGameplayModifierInfo& ModifierInfo = GameplayEffect->Modifiers[Idx];
		ModifierInfo.Attribute.SetUProperty(UMyAttributeSet::GetMyModifiedAttribute());
		ModifierInfo.ModifierMagnitude = FScalableFloat(42.f);
		ModifierInfo.ModifierOp = EGameplayModOp::Override;

		// Apply the GE.

		// Create the GESpec here to avoid the behavior of ASC to create GESpecs from the GE class default object.
		// Since we have a dynamic GE here, this would create a GESpec with the base GameplayEffect class, so we
		// would lose our modifiers. Attention: It is unknown, if this "hack" done here can have drawbacks!
		// The spec prevents the GE object being collected by the GarbageCollector, since the GE is a UPROPERTY on the spec.
		FGameplayEffectSpec* GESpec = new FGameplayEffectSpec(GameplayEffect, {}, 0.f); // "new", since lifetime is managed by a shared ptr within the handle
		ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, FGameplayEffectSpecHandle(GESpec));
	}
	EndAbility(Handle, ActorInfo, ActivationInfo, false, false);
}
```

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ge-containers"></a>
#### 4.5.18 Gameplay Effect Containers
Epic 的 [动作角色扮演示例项目](https://www.unrealengine.com/marketplace/en-US/product/action-rpg) 实现了一种名为 `FGameplayEffectContainer` 的结构。它们不在普通 GAS 中，但非常方便地容纳 `GameplayEffects` 和 [`TargetData`](#concepts-targeting-data)。它自动执行一些工作，例如从 `GameplayEffects` 创建 `GameplayEffectSpecs` 并在其 `GameplayEffectContext` 中设置默认值。在 `GameplayAbility` 中制作 `GameplayEffectContainer` 并将其传递给生成的射弹非常简单明了。我选择不在随附的示例项目中实现 `GameplayEffectContainers`，以展示在普通 GAS 中没有它们时您将如何工作，但我强烈建议您研究它们并考虑将它们添加到您的项目中。

要访问 `GameplayEffectContainers` 内部的 `GESpecs` 来执行添加 `SetByCallers` 等操作，请中断 `FGameplayEffectContainer` 并通过 `GESpecs` 数组中的索引访问 `GESpec` 引用。这要求您提前知道要访问的 `GESpec` 的索引。

![SetByCaller with a GameplayEffectContainer](https://github.com/tranek/GASDocumentation/raw/master/Images/gecontainersetbycaller.png)

`GameplayEffectContainers`还包含[targeting](#concepts-targeting-containers)的可选高效装置。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ga"></a>
### 4.6 Gameplay Abilities（GA）

<a name="concepts-ga-definition"></a>
#### 4.6.1 Gameplay Ability 的定义
[`GameplayAbilities`](https://docs.unrealengine.com/en-US/API/Plugins/GameplayAbilities/Abilities/UGameplayAbility/index.html) (`GA`) 是 `Actor` 在游戏中可以执行的任何动作或技能。可以同时激活多个 `GameplayAbility`，例如冲刺和开枪。这些可以用蓝图或 C++ 制作。

`GameplayAbilities` 的示例：
* 跳跃
* 短跑
* 开枪
* 每 X 秒被动阻止一次攻击
* 使用药水
* 打开一扇门
* 收集资源
* 建造一座建筑物

不应该用 `GameplayAbilities` 实现的事情：
* 基本动作输入
* 与 UI 的一些交互 - 不要使用 `GameplayAbility` 从商店购买商品。

这些不是规则，只是我的建议。您的设计和实现可能会有所不同。

`GameplayAbilities` 附带默认功能，具有修改属性更改量或更改 `GameplayAbility's` 功能的级别。

`GameplayAbilities` 在拥有的客户端和/或服务器上运行，具体取决于 [`Net Execution Policy`](#concepts-ga-net)，但不是模拟代理。 `Net Execution Policy` 确定 `GameplayAbility` 是否为本地 [predicted](#concepts-p)。它们包括 [optional cost and cooldown `GameplayEffects`](#concepts-ga-commit) 的默认行为。 `GameplayAbilities` 使用 [`AbilityTasks`](#concepts-at) 来执行随时间推移发生的操作，例如等待事件、等待属性更改、等待玩家选择目标或使用 `Root Motion Source` 移动 `Character`。 **模拟客户端将不会运行 `GameplayAbilities`**。相反，当服务器运行该功能时，任何视觉上需要在模拟代理上播放的内容（例如Animation Montage）都将通过 `AbilityTasks` 或 [`GameplayCues`](#concepts-gc) 进行复制或 RPC，以实现声音和粒子等装饰性内容。

所有 `GameplayAbilities` 的 `ActivateAbility()` 功能都会被您的游戏逻辑覆盖。可以向 `EndAbility()` 添加附加逻辑，该逻辑在 `GameplayAbility` 完成或取消时运行。

简单`GameplayAbility`的流程图：
![Simple GameplayAbility Flowchart](https://github.com/tranek/GASDocumentation/raw/master/Images/abilityflowchartsimple.png)


更复杂的`GameplayAbility`的流程图：
![Complex GameplayAbility Flowchart](https://github.com/tranek/GASDocumentation/raw/master/Images/abilityflowchartcomplex.png)

可以使用多个相互交互（激活、取消等）的 `GameplayAbilities` 来实现复杂的能力。

<a name="concepts-ga-definition-reppolicy"></a>
##### 4.6.1.1 Replication Policy（复制策略）
不要使用此选项。该名称具有误导性，您不需要它。默认情况下，[`GameplayAbilitySpecs`](#concepts-ga-spec) 从服务器复制到所属客户端。如上所述，**`GameplayAbilities` 不在模拟代理上运行**。他们使用 `AbilityTasks` 和 `GameplayCues` 来复制或 RPC 对模拟代理的视觉更改。 Epic 的 Dave Ratti 向 [将来删除此选项](https://epicgames.ent.box.com/s/m1egifkxv3he3u3xezb9hzbgroxyhx89) 表达了他的愿望。

<a name="concepts-ga-definition-remotecancel"></a>
##### 4.6.1.2 Server Respects Remote Ability Cancellation
这个选项常常会带来麻烦。这意味着如果客户端的 `GameplayAbility` 由于取消或自然完成而结束，则无论是否完成都会强制服务器的版本结束。后一个问题很重要，特别是对于高延迟玩家使用的本地预测的 `GameplayAbilities` 而言。通常您会想要禁用此选项。

<a name="concepts-ga-definition-repinputdirectly"></a>
##### 4.6.1.3 Replicate Input Directly
设置此选项将始终将输入按下和释放事件复制到服务器。 Epic 建议不要使用此选项，而是依赖内置于现有输入相关 [`AbilityTasks`](#concepts-at) 中的 `Generic Replicated Events`（如果您有 [input bound to your `ASC`](#concepts-ga-input)）。

史诗评论：
```c++
/** Direct Input state replication. These will be called if bReplicateInputDirectly is true on the ability and is generally not a good thing to use. (Instead, prefer to use Generic Replicated Events). */
UAbilitySystemComponent::ServerSetInputPressed()
```

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ga-input"></a>
#### 4.6.2 将输入绑定到 ASC
`ASC` 允许您直接将输入操作绑定到它，并在授予这些输入时将这些输入分配给 `GameplayAbilities`。如果满足 `GameplayTag` 要求，分配给 `GameplayAbilities` 的输入操作会在按下时自动激活这些 `GameplayAbilities`。需要分配的输入操作才能使用响应输入的内置 `AbilityTasks`。

除了分配用于激活 `GameplayAbilities` 的输入操作外，`ASC` 还接受通用 `Confirm` 和 `Cancel` 输入。 `AbilityTasks` 使用这些特殊输入来确认或取消 [`Target Actors`](#concepts-targeting-actors) 等内容。

要将输入绑定到 `ASC`，您必须首先创建一个将输入操作名称转换为字节的枚举。枚举名称必须与项目设置中用于输入操作的名称完全匹配。 `DisplayName` 并不重要。

来自示例项目：
```c++
UENUM(BlueprintType)
enum class EGDAbilityInputID : uint8
{
	// 0 None
	None			UMETA(DisplayName = "None"),
	// 1 Confirm
	Confirm			UMETA(DisplayName = "Confirm"),
	// 2 Cancel
	Cancel			UMETA(DisplayName = "Cancel"),
	// 3 LMB
	Ability1		UMETA(DisplayName = "Ability1"),
	// 4 RMB
	Ability2		UMETA(DisplayName = "Ability2"),
	// 5 Q
	Ability3		UMETA(DisplayName = "Ability3"),
	// 6 E
	Ability4		UMETA(DisplayName = "Ability4"),
	// 7 R
	Ability5		UMETA(DisplayName = "Ability5"),
	// 8 Sprint
	Sprint			UMETA(DisplayName = "Sprint"),
	// 9 Jump
	Jump			UMETA(DisplayName = "Jump")
};
```

如果您的 `ASC` 位于 `Character` 上，则在 `SetupPlayerInputComponent()` 中包含用于绑定到 `ASC` 的函数：
```c++
// Bind to AbilitySystemComponent
FTopLevelAssetPath AbilityEnumAssetPath = FTopLevelAssetPath(FName("/Script/GASDocumentation"), FName("EGDAbilityInputID"));
AbilitySystemComponent->BindAbilityActivationToInputComponent(PlayerInputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
	FString("CancelTarget"), AbilityEnumAssetPath, static_cast<int32>(EGDAbilityInputID::Confirm), static_cast<int32>(EGDAbilityInputID::Cancel)));
```

如果您的 `ASC` 位于 `PlayerState` 上，则 `SetupPlayerInputComponent()` 内部存在潜在的竞争条件，其中 `PlayerState` 可能尚未复制到客户端。因此，我建议尝试绑定到 `SetupPlayerInputComponent()` 和 `OnRep_PlayerState()` 中的输入。 `OnRep_PlayerState()` 本身是不够的，因为在 `PlayerController` 告诉客户端调用创建 `InputComponent` 的 `ClientRestart()` 之前进行 `PlayerState` 复制时，`Actor's` `InputComponent` 可能为空。示例项目演示了尝试通过布尔门控过程在两个位置进行绑定，因此它实际上只绑定输入一次。

**注意：** 在示例项目中，枚举中的 `Confirm` 和 `Cancel` 与项目设置中的输入操作名称（`ConfirmTarget` 和 `CancelTarget`）不匹配，但我们在 `BindAbilityActivationToInputComponent()` 中提供它们之间的映射。这些很特殊，因为我们提供映射并且它们不必匹配，但它们可以匹配。枚举中的所有其他输入必须与项目设置中的输入操作名称匹配。

对于仅由一个输入激活的 `GameplayAbilities`（它们始终像 MOBA 一样存在于同一个“槽”中），我更喜欢向我的 `UGameplayAbility` 子类添加一个变量，我可以在其中定义它们的输入。然后我可以在授予该能力时从 `ClassDefaultObject` 中读取此内容。

<a name="concepts-ga-input-noactivate"></a>
##### 4.6.2.1 绑定输入但不激活 Ability
如果您不希望 `GameplayAbilities` 在按下输入时自动激活，但仍将它们绑定到输入以与 `AbilityTasks` 一起使用，则可以向 `UGameplayAbility` 子类 `bActivateOnInput` 添加新的 bool 变量，该变量默认为 `true` 并覆盖 `UAbilitySystemComponent::AbilityLocalInputPressed()`。

```c++
void UGSAbilitySystemComponent::AbilityLocalInputPressed(int32 InputID)
{
	// Consume the input if this InputID is overloaded with GenericConfirm/Cancel and the GenericConfim/Cancel callback is bound
	if (IsGenericConfirmInputBound(InputID))
	{
		LocalInputConfirm();
		return;
	}

	if (IsGenericCancelInputBound(InputID))
	{
		LocalInputCancel();
		return;
	}

	// ---------------------------------------------------------

	ABILITYLIST_SCOPE_LOCK();
	for (FGameplayAbilitySpec& Spec : ActivatableAbilities.Items)
	{
		if (Spec.InputID == InputID)
		{
			if (Spec.Ability)
			{
				Spec.InputPressed = true;
				if (Spec.IsActive())
				{
					if (Spec.Ability->bReplicateInputDirectly && IsOwnerActorAuthoritative() == false)
					{
						ServerSetInputPressed(Spec.Handle);
					}

					AbilitySpecInputPressed(Spec);

					// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
					InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
				}
				else
				{
					UGSGameplayAbility* GA = Cast<UGSGameplayAbility>(Spec.Ability);
					if (GA && GA->bActivateOnInput)
					{
						// Ability is not active, so try to activate it
						TryActivateAbility(Spec.Handle);
					}
				}
			}
		}
	}
}
```

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ga-granting"></a>
#### 4.6.3 授予 Abilities
将 `GameplayAbility` 授予 `ASC` 会将其添加到 `ActivatableAbilities` 的 `ASC's` 列表中，从而允许它在满足 [`GameplayTag` requirements](#concepts-ga-tags) 的情况下随意激活 `GameplayAbility`。

我们在服务器上授予 `GameplayAbilities`，然后服务器会自动将 [`GameplayAbilitySpec`](#concepts-ga-spec) 复制到拥有的客户端。其他客户端/模拟代理不会收到 `GameplayAbilitySpec`。

示例项目将 `TArray<TSubclassOf<UGDGameplayAbility>>` 存储在 `Character` 类上，它会在游戏开始时读取并授予该类：
```c++
void AGDCharacterBase::AddCharacterAbilities()
{
	// Grant abilities, but only on the server	
	if (Role != ROLE_Authority || !AbilitySystemComponent.IsValid() || AbilitySystemComponent->bCharacterAbilitiesGiven)
	{
		return;
	}

	for (TSubclassOf<UGDGameplayAbility>& StartupAbility : CharacterAbilities)
	{
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(StartupAbility, GetAbilityLevel(StartupAbility.GetDefaultObject()->AbilityID), static_cast<int32>(StartupAbility.GetDefaultObject()->AbilityInputID), this));
	}

	AbilitySystemComponent->bCharacterAbilitiesGiven = true;
}
```

当授予这些 `GameplayAbilities` 时，我们将使用 `UGameplayAbility` 类、能力级别、绑定到的输入以及 `SourceObject` 或谁将此 `GameplayAbility` 赋予此 `ASC` 来创建 `GameplayAbilitySpecs`。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ga-activating"></a>
#### 4.6.4 激活 Abilities
如果为 `GameplayAbility` 分配了输入动作，则按下输入并且满足其 `GameplayTag` 要求时，它将自动激活。这可能并不总是激活 `GameplayAbility` 的理想方法。 `ASC` 提供了四种激活 `GameplayAbilities` 的方法：通过 `GameplayTag`、`GameplayAbility` 类、`GameplayAbilitySpec` 句柄和通过事件。通过事件激活 `GameplayAbility` 允许您 [通过事件传递数据负载](#concepts-ga-data)。

```c++
UFUNCTION(BlueprintCallable, Category = "Abilities")
bool TryActivateAbilitiesByTag(const FGameplayTagContainer& GameplayTagContainer, bool bAllowRemoteActivation = true);

UFUNCTION(BlueprintCallable, Category = "Abilities")
bool TryActivateAbilityByClass(TSubclassOf<UGameplayAbility> InAbilityToActivate, bool bAllowRemoteActivation = true);

bool TryActivateAbility(FGameplayAbilitySpecHandle AbilityToActivate, bool bAllowRemoteActivation = true);

bool TriggerAbilityFromGameplayEvent(FGameplayAbilitySpecHandle AbilityToTrigger, FGameplayAbilityActorInfo* ActorInfo, FGameplayTag Tag, const FGameplayEventData* Payload, UAbilitySystemComponent& Component);

FGameplayAbilitySpecHandle GiveAbilityAndActivateOnce(const FGameplayAbilitySpec& AbilitySpec, const FGameplayEventData* GameplayEventData);
```
要通过事件激活 `GameplayAbility`，`GameplayAbility` 必须在 `GameplayAbility` 中设置其 `Triggers`。分配 `GameplayTag` 并为 `GameplayEvent` 选择一个选项。要发送事件，请使用函数 `UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(AActor* Actor, FGameplayTag EventTag, FGameplayEventData Payload)`。通过事件激活 `GameplayAbility` 允许您传入带有数据的有效负载。

`GameplayAbility` `Triggers` 还允许您在添加或删除 `GameplayTag` 时激活 `GameplayAbility`。

**注意：** 当从蓝图中的事件激活 `GameplayAbility` 时，您必须使用 `ActivateAbilityFromEvent` 节点。

**注意：** 当 `GameplayAbility` 应该终止时，不要忘记调用 `EndAbility()`，除非你有一个总是像被动能力一样运行的 `GameplayAbility`。

**本地预测** `GameplayAbilities` 的激活序列：
1. **拥有客户端**致电 `TryActivateAbility()`
1. 呼叫 `InternalTryActivateAbility()`
1. 调用 `CanActivateAbility()` 并返回是否满足 `GameplayTag` 要求、`ASC` 是否能够负担成本、`GameplayAbility` 未处于冷却状态以及当前没有其他实例处于活动状态
1. 调用 `CallServerTryActivateAbility()` 并将其生成的 `Prediction Key` 传递给它
1. 呼叫 `CallActivateAbility()`
1. 调用 `PreActivate()` Epic 将此称为“样板初始化内容”
1. 呼叫`ActivateAbility()`最终激活能力

**服务器**收到`CallServerTryActivateAbility()`
1. 呼叫 `ServerTryActivateAbility()`
1. 呼叫 `InternalServerTryActivateAbility()` 
1. 呼叫 `InternalTryActivateAbility()`
1. 调用 `CanActivateAbility()` 并返回是否满足 `GameplayTag` 要求、`ASC` 是否能够负担成本、`GameplayAbility` 未处于冷却状态以及当前没有其他实例处于活动状态
1. 如果成功，则调用 `ClientActivateAbilitySucceed()`，告诉它更新其 `ActivationInfo`，服务器已确认其激活并广播 `OnConfirmDelegate` 委托。这与输入确认不同。
1. 呼叫 `CallActivateAbility()`
1. 调用 `PreActivate()` Epic 将此称为“样板初始化内容”
1. 呼叫`ActivateAbility()`最终激活能力

如果服务器在任何时候无法激活，它将调用 `ClientActivateAbilityFailed()`，立即终止客户端的 `GameplayAbility` 并撤消任何预测的更改。

<a name="concepts-ga-activating-passive"></a>
##### 4.6.4.1 被动 Abilities
要实现自动激活并持续运行的被动 `GameplayAbilities`，可以重写 `UGameplayAbility::OnAvatarSet()`，并在其中调用 `TryActivateAbility()`。当 `GameplayAbility` 被授予且 `AvatarActor` 已设置时，会调用该函数。

我建议将 `bool` 添加到您的自定义 `UGameplayAbility` 类，指定在授予时是否应激活 `GameplayAbility`。示例项目这样做是因为其被动装甲堆叠能力。

无源 `GameplayAbilities` 通常具有 [`Net Execution Policy`](#concepts-ga-net) 或 `Server Only`。

```c++
void UGDGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilitySpec & Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (bActivateAbilityOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}
```

Epic 将此功能描述为启动被动能力和执行 `BeginPlay` 类型事情的正确位置。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ga-activating-failedtags"></a>
##### 4.6.4.2 激活失败的 Tags

技能有默认逻辑来告诉您技能激活失败的原因。要启用此功能，您必须设置与默认失败情况相对应的 GameplayTags。

将这些标签（或您自己的命名约定）添加到您的项目中：
```
+GameplayTagList=(Tag="Activation.Fail.BlockedByTags",DevComment="")
+GameplayTagList=(Tag="Activation.Fail.CantAffordCost",DevComment="")
+GameplayTagList=(Tag="Activation.Fail.IsDead",DevComment="")
+GameplayTagList=(Tag="Activation.Fail.MissingTags",DevComment="")
+GameplayTagList=(Tag="Activation.Fail.Networking",DevComment="")
+GameplayTagList=(Tag="Activation.Fail.OnCooldown",DevComment="")
```

然后将它们添加到[`GASDocumentation\Config\DefaultGame.ini`](https://github.com/tranek/GASDocumentation/blob/master/Config/DefaultGame.ini#L8-L13)：
```
[/Script/GameplayAbilities.AbilitySystemGlobals]
ActivateFailIsDeadName=Activation.Fail.IsDead
ActivateFailCooldownName=Activation.Fail.OnCooldown
ActivateFailCostName=Activation.Fail.CantAffordCost
ActivateFailTagsBlockedName=Activation.Fail.BlockedByTags
ActivateFailTagsMissingName=Activation.Fail.MissingTags
ActivateFailNetworkingName=Activation.Fail.Networking
```

现在，每当能力激活失败时，相应的 GameplayTag 将包含在输出日志消息中或在 `showdebug AbilitySystem` HUD 上可见。
```
LogAbilitySystem: Display: InternalServerTryActivateAbility. Rejecting ClientActivation of Default__GA_FireGun_C. InternalTryActivateAbility failed: Activation.Fail.BlockedByTags
LogAbilitySystem: Display: ClientActivateAbilityFailed_Implementation. PredictionKey :109 Ability: Default__GA_FireGun_C
```

![Activation Failed Tags Displayed in showdebug AbilitySystem](https://github.com/tranek/GASDocumentation/raw/master/Images/activationfailedtags.png)

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ga-cancelabilities"></a>
#### 4.6.5 取消 Abilities
要从内部取消 `GameplayAbility`，请调用 `CancelAbility()`。这将调用 `EndAbility()` 并将其 `WasCancelled` 参数设置为 true。

为了从外部取消 `GameplayAbility`，`ASC` 提供了一些功能：

```c++
/** Cancels the specified ability CDO. */
void CancelAbility(UGameplayAbility* Ability);	

/** Cancels the ability indicated by passed in spec handle. If handle is not found among reactivated abilities nothing happens. */
void CancelAbilityHandle(const FGameplayAbilitySpecHandle& AbilityHandle);

/** Cancel all abilities with the specified tags. Will not cancel the Ignore instance */
void CancelAbilities(const FGameplayTagContainer* WithTags=nullptr, const FGameplayTagContainer* WithoutTags=nullptr, UGameplayAbility* Ignore=nullptr);

/** Cancels all abilities regardless of tags. Will not cancel the ignore instance */
void CancelAllAbilities(UGameplayAbility* Ignore=nullptr);

/** Cancels all abilities and kills any remaining instanced abilities */
virtual void DestroyActiveState();
```

**注意：** 我发现如果您有 `Non-Instanced` `GameplayAbilities`，`CancelAllAbilities` 似乎无法正常工作。似乎打到了`Non-Instanced` `GameplayAbility`就放弃了。 `CancelAbilities` 可以更好地处理 `Non-Instanced` `GameplayAbilities`，这就是示例项目所使用的（Jump 是非实例 `GameplayAbility`）。您的里程可能会有所不同。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ga-definition-activeability"></a>
#### 4.6.6 获取激活中的 Abilities
初学者经常会问“如何获得主动能力？”也许可以在其上设置变量或取消它。一次可以激活多个 `GameplayAbility`，因此不存在一种“激活能力”。相反，您必须搜索 `ActivatableAbilities` 的 `ASC's` 列表（假设 `ASC` 拥有 `GameplayAbilities`），并找到与您要查找的 [`Asset` or `Granted` `GameplayTag`](#concepts-ga-tags) 匹配的列表。

`UAbilitySystemComponent::GetActivatableAbilities()` 返回 `TArray<FGameplayAbilitySpec>` 供您迭代。

`ASC` 还有另一个辅助函数，它接受 `GameplayTagContainer` 作为参数来辅助搜索，而不是手动迭代 `GameplayAbilitySpecs` 列表。 `bOnlyAbilitiesThatSatisfyTagRequirements` 参数将仅返回满足 `GameplayTag` 要求并且可以立即激活的 `GameplayAbilitySpecs`。例如，您可以有两种基本攻击 `GameplayAbilities`，一种使用武器，一种使用赤手空拳，并且根据是否配备武器设置 `GameplayTag` 要求来激活正确的攻击。有关更多信息，请参阅 Epic 对该功能的评论。
```c++
UAbilitySystemComponent::GetActivatableGameplayAbilitySpecsByAllMatchingTags(const FGameplayTagContainer& GameplayTagContainer, TArray < struct FGameplayAbilitySpec* >& MatchingGameplayAbilities, bool bOnlyAbilitiesThatSatisfyTagRequirements = true)
```

获得所需的 `FGameplayAbilitySpec` 后，您可以在其上调用 `IsActive()`。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ga-instancing"></a>
#### 4.6.7 Instancing Policy（实例化策略）
`GameplayAbility's` `Instancing Policy` 确定 `GameplayAbility` 在激活时是否实例化以及如何实例化。

| `Instancing Policy`     | 描述                                                                                      | 何时使用的示例                                                                                                                                                                                                                                                                                                                                                                                             |
| ----------------------- | ------------------------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| 每个Actor实例化     | 每个 `ASC` 仅具有一个在激活之间重复使用的 `GameplayAbility` 实例。    | 这可能是您最常使用的 `Instancing Policy`。您可以将其用于任何能力，并在激活之间提供持久性。设计者负责在需要它的激活之间手动重置任何变量。                                                                                                                                                               |
| 每次执行实例化 | 每次激活 `GameplayAbility` 时，都会创建 `GameplayAbility` 的新实例。 | 这些 `GameplayAbilities` 的好处是每次激活时变量都会重置。它们提供的性能比 `Instanced Per Actor` 更差，因为它们每次激活时都会生成新的 `GameplayAbilities`。示例项目不使用其中任何一个。                                                                                                                                 |
| 非实例化           | `GameplayAbility` 在其 `ClassDefaultObject` 上运行。没有创建任何实例。            | 这是三者中性能最好的，但它的用途也是最受限制的。 `Non-Instanced` `GameplayAbilities` 无法存储状态，这意味着没有动态变量，也没有绑定到 `AbilityTask` 委托。使用它们的最佳位置是经常使用的简单能力，例如 MOBA 或 RTS 中的小兵基本攻击。示例项目的跳转 `GameplayAbility` 是 `Non-Instanced`。 |

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ga-net"></a>
#### 4.6.8 Net Execution Policy（网络执行策略）
`GameplayAbility's` `Net Execution Policy` 确定谁运行 `GameplayAbility` 以及运行顺序。

| `Net Execution Policy` | 描述                                                                                                                                                                                                         |
| ---------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `Local Only`           | `GameplayAbility` 仅在所属客户端上运行。这对于仅进行局部外观更改的能力可能很有用。单人游戏应使用 `Server Only`。                                     |
| `Local Predicted`      | `Local Predicted` `GameplayAbilities` 首先在所属客户端上激活，然后在服务器上激活。服务器的版本将纠正客户端错误预测的任何内容。参见 [预测](#concepts-p)。 |
| `Server Only`          | `GameplayAbility` 仅在服务器上运行。无源 `GameplayAbilities` 通常为 `Server Only`。单人游戏应该使用这个。                                                                  |
| `Server Initiated`     | `Server Initiated` `GameplayAbilities` 首先在服务器上激活，然后在所属客户端上激活。我个人并没有使用过这些（如果有的话）。                                                                     | 

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ga-tags"></a>
#### 4.6.9 Ability Tags
`GameplayAbilities` 配有内置逻辑的 `GameplayTagContainers`。这些 `GameplayTags` 都没有被复制。

| `GameplayTag Container`     | 描述                                                                                                                                                                                   |
| --------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `Ability Tags`              | `GameplayAbility` 拥有的 `GameplayTags`。这些只是`GameplayTags`对`GameplayAbility`的描述。                                                                              |
| `Cancel Abilities with Tag` | 当此 `GameplayAbility` 激活时，在其 `Ability Tags` 中具有这些 `GameplayTags` 的其他 `GameplayAbilities` 将被取消。                                                   |
| `Block Abilities with Tag`  | 当此 `GameplayAbility` 处于活动状态时，在其 `Ability Tags` 中具有这些 `GameplayTags` 的其他 `GameplayAbilities` 将被阻止激活。                                          |
| `Activation Owned Tags`     | 当此 `GameplayAbility` 处于活动状态时，这些 `GameplayTags` 将提供给 `GameplayAbility's` 所有者。请记住，这些内容不会被复制。                                                    |
| `Activation Required Tags`  | 仅当所有者拥有**所有**这些 `GameplayTags` 时，才能激活此 `GameplayAbility`。                                                                                                |
| `Activation Blocked Tags`   | 如果所有者拥有这些 `GameplayTags` 中的**任何**，则无法激活该 `GameplayAbility`。                                                                                                  |
| `Source Required Tags`      | 仅当 `Source` 具有这些 `GameplayTags` 的**全部**时，才能激活此 `GameplayAbility`。仅当 `GameplayAbility` 被事件触发时，`Source` `GameplayTags` 才会被设置。 |
| `Source Blocked Tags`       | 如果 `Source` 具有这些 `GameplayTags` 中的**任何**，则无法激活该 `GameplayAbility`。仅当 `GameplayAbility` 被事件触发时，`Source` `GameplayTags` 才会被设置。   |
| `Target Required Tags`      | 仅当 `Target` 具有这些 `GameplayTags` 的**全部**时，才能激活此 `GameplayAbility`。仅当 `GameplayAbility` 被事件触发时，`Target` `GameplayTags` 才会被设置。 |
| `Target Blocked Tags`       | 如果 `Target` 具有这些 `GameplayTags` 中的**任何**，则无法激活该 `GameplayAbility`。仅当 `GameplayAbility` 被事件触发时，`Target` `GameplayTags` 才会被设置。   |

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ga-spec"></a>
#### 4.6.10 Gameplay Ability Spec
在授予 `GameplayAbility` 后，`GameplayAbilitySpec` 存在于 `ASC` 上，并定义可激活的 `GameplayAbility` - `GameplayAbility` 类、级别、输入绑定和运行时状态，必须与 `GameplayAbility` 类分开。

当在服务器上授予 `GameplayAbility` 时，服务器会将 `GameplayAbilitySpec` 复制到拥有的客户端，以便她可以激活它。

激活 `GameplayAbilitySpec` 将创建 `GameplayAbility` 的实例（或不创建 `Non-Instanced` `GameplayAbilities`），具体取决于其 `Instancing Policy`。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ga-data"></a>
#### 4.6.11 向 Ability 传递数据
`GameplayAbilities` 的通用范例是 `Activate->Generate Data->Apply->End`。有时您需要根据现有数据采取行动。 GAS 提供了一些将外部数据导入 `GameplayAbilities` 的选项：

| 方法                                          | 描述                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   |
| ----------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 通过事件激活`GameplayAbility`             | 使用包含数据负载的事件激活 `GameplayAbility`。事件的有效负载从客户端复制到服务器，以获取本地预测的 `GameplayAbilities`。对于不适合任何现有变量的任意数据，使用两个 `Optional Object` 或 [`TargetData`](#concepts-targeting-data) 变量。这样做的缺点是它会阻止您通过输入绑定激活该功能。要通过事件激活 `GameplayAbility`，`GameplayAbility` 必须在 `GameplayAbility` 中设置其 `Triggers`。分配 `GameplayTag` 并为 `GameplayEvent` 选择一个选项。要发送事件，请使用函数 `UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(AActor* Actor, FGameplayTag EventTag, FGameplayEventData Payload)`。 |
| 使用 `WaitGameplayEvent` `AbilityTask`           | 使用 `WaitGameplayEvent` `AbilityTask` 告诉 `GameplayAbility` 在激活后监听带有负载数据的事件。事件负载及其发送过程与通过事件激活 `GameplayAbilities` 相同。这样做的缺点是事件不会被 `AbilityTask` 复制，并且只能用于 `Local Only` 和 `Server Only` `GameplayAbilities`。您可能可以编写自己的 `AbilityTask` 来复制事件负载。                                                                                                                                                                                                                                                                                               |
| 使用`TargetData`                                | 自定义 `TargetData` 结构是在客户端和服务器之间传递任意数据的好方法。                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              |
| 将数据存储在`OwnerActor`或`AvatarActor`上 | 使用存储在 `OwnerActor`、`AvatarActor` 或您可以获得引用的任何其他对象上的复制变量。此方法最灵活，可与输入绑定激活的 `GameplayAbilities` 一起使用。但是，它不保证数据在使用时会从复制同步。您必须提前确保这一点 - 这意味着如果您设置复制变量然后立即激活 `GameplayAbility`，则由于潜在的数据包丢失，无法保证接收器上发生的顺序。                                                                                                                                                                                                                                   |

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ga-commit"></a>
#### 4.6.12 Ability 的消耗与冷却
`GameplayAbilities` 具有可选成本和冷却时间的功能。成本是 `ASC` 必须具有的 `Attributes` 的预定义金额，以便激活通过 `Instant` `GameplayEffect` ([`Cost GE`](#concepts-ge-cost)) 实现的 `GameplayAbility`。冷却时间是计时器，可防止 `GameplayAbility` 重新激活，直至其过期，并通过 `Duration` `GameplayEffect` ([`Cooldown GE`](#concepts-ge-cooldown)) 实现。

在 `GameplayAbility` 调用 `UGameplayAbility::Activate()` 之前，它会调用 `UGameplayAbility::CanActivateAbility()`。此功能检查拥有的 `ASC` 是否能够负担成本（`UGameplayAbility::CheckCost()`）并确保 `GameplayAbility` 未处于冷却状态（`UGameplayAbility::CheckCooldown()`）。

`GameplayAbility` 调用 `Activate()` 后，它可以随时使用调用 `UGameplayAbility::CommitCost()` 和 `UGameplayAbility::CommitCooldown()` 的 `UGameplayAbility::CommitAbility()` 选择性地提交成本和冷却时间。如果不应同时提交，设计者可以选择分别调用 `CommitCost()` 或 `CommitCooldown()`。承诺成本和冷却时间再次调用 `CheckCost()` 和 `CheckCooldown()`，这是 `GameplayAbility` 与它们相关的失败的最后机会。在激活 `GameplayAbility` 后，拥有的 `ASC's` `Attributes` 可能会发生变化，无法满足提交时的成本。如果 [预测键](#concepts-p-key) 在提交时有效，则提交成本和冷却时间可以为 [局部预测](#concepts-p)。

有关实现细节，请参阅 [`CostGE`](#concepts-ge-cost) 和 [`CooldownGE`](#concepts-ge-cooldown)。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ga-leveling"></a>
#### 4.6.13 提升 Ability 的等级
提升能力的常用方法有两种：

| 升级方法                            | 描述                                                                                                                                                                                                      |
| ------------------------------------------ | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 新级别的取消授予和重新授予       | 取消授予（删除）`GameplayAbility`从`ASC`并在服务器上的下一个级别重新授予它。这将终止`GameplayAbility`如果当时它是活跃的。                                   |
| 提高`GameplayAbilitySpec's`等级 | 在服务器上，找到`GameplayAbilitySpec`，增加其级别，并将其标记为脏，以便复制到拥有的客户端。该方法不会终止`GameplayAbility`如果当时它是活跃的。 |

两种方法的主要区别在于是否希望在升级时取消激活的`GameplayAbilities`。您很可能会使用这两种方法，具体取决于您的 `GameplayAbilities`。我建议将 `bool` 添加到 `UGameplayAbility` 子类中，指定要使用的方法。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ga-sets"></a>
#### 4.6.14 Ability Sets
`GameplayAbilitySets` 是方便的 `UDataAsset` 类，用于保存输入绑定和启动 `GameplayAbilities` 列表，用于具有授予 `GameplayAbilities` 逻辑的字符。子类还可以包含额外的逻辑或属性。 Paragon 每个英雄都有一个 `GameplayAbilitySet`，其中包括所有给定的 `GameplayAbilities`。

我发现这门课是不必要的，至少从我到目前为止所看到的来看是这样。示例项目处理 `GDCharacterBase` 及其子类内部 `GameplayAbilitySets` 的所有功能。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ga-batching"></a>
#### 4.6.15 Ability 批处理
传统的 `Gameplay Ability` 生命周期涉及从客户端到服务器的至少两到三个 RPC。

1. `CallServerTryActivateAbility()`
1. `ServerSetReplicatedTargetData()`（可选）
1. `ServerEndAbility()`

如果 `GameplayAbility` 在一个帧中的一个原子分组中执行所有这些操作，我们可以优化此工作流程，将所有两个或三个 RPC 批处理（组合）到一个 RPC 中。 `GAS` 将此 RPC 优化称为 `Ability Batching`。何时使用 `Ability Batching` 的常见示例是用于命中扫描枪。命中扫描枪激活，进行线路跟踪，将 [`TargetData`](#concepts-targeting-data) 发送到服务器，并在一帧中将所有能力全部集中在一个原子组中。 [GASShooter](https://github.com/tranek/GASShooter) 示例项目演示了其命中扫描枪的这种技术。

半自动枪是最好的情况，并将 `CallServerTryActivateAbility()`、`ServerSetReplicatedTargetData()`（子弹击中结果）和 `ServerEndAbility()` 批量放入一个 RPC，而不是三个 RPC。

全自动/连发枪将 `CallServerTryActivateAbility()` 和 `ServerSetReplicatedTargetData()` 的第一颗子弹批量放入一个 RPC，而不是两个 RPC。随后的每个项目符号都是其自己的 `ServerSetReplicatedTargetData()` RPC。最后，当枪停止射击时，`ServerEndAbility()` 作为单独的 RPC 发送。这是最坏的情况，我们只在第一个项目符号上保存一个 RPC，而不是两个。此场景也可以通过通过 [`Gameplay Event`](#concepts-ga-data) 激活该功能来实现，该功能会将子弹的 `TargetData` 与 `EventPayload` 一起从客户端发送到服务器。后一种方法的缺点是 `TargetData` 必须在功能外部生成，而批处理方法则在功能内部生成 `TargetData`。

`Ability Batching` 在 [`ASC`](#concepts-asc) 上默认处于禁用状态。要启用 `Ability Batching`，请覆盖 `ShouldDoServerAbilityRPCBatch()` 以返回 true：

```c++
virtual bool ShouldDoServerAbilityRPCBatch() const override { return true; }
```

现在 `Ability Batching` 已启用，在激活您想要批量的能力之前，您必须事先创建一个 `FScopedServerAbilityRPCBatcher` 结构体。这个特殊的结构将尝试在其范围内批处理其后的任何能力。一旦 `FScopedServerAbilityRPCBatcher` 超出范围，任何激活的能力都不会尝试批量。 `FScopedServerAbilityRPCBatcher` 的工作原理是在每个可批处理的函数中包含特殊代码，拦截发送 RPC 的调用，并将消息打包到批处理结构中。当 `FScopedServerAbilityRPCBatcher` 超出范围时，它会自动将此批处理结构 RPC 到 `UAbilitySystemComponent::EndServerAbilityRPCBatch()` 中的服务器。服务器接收`UAbilitySystemComponent::ServerAbilityRPCBatch_Internal(FServerAbilityRPCBatch& BatchInfo)`中的批量RPC。 `BatchInfo` 参数将包含用于指示该能力是否应结束以及在激活时是否按下输入的标志，以及 `TargetData`（如果包含）。这是一个很好的函数，可以设置断点来确认批处理是否正常工作。或者，使用 cvar `AbilitySystem.ServerRPCBatching.Log 1` 启用特殊功能批处理日志记录。

这个机制只能用C++实现，并且只能通过`FGameplayAbilitySpecHandle`激活能力。

```c++
bool UGSAbilitySystemComponent::BatchRPCTryActivateAbility(FGameplayAbilitySpecHandle InAbilityHandle, bool EndAbilityImmediately)
{
	bool AbilityActivated = false;
	if (InAbilityHandle.IsValid())
	{
		FScopedServerAbilityRPCBatcher GSAbilityRPCBatcher(this, InAbilityHandle);
		AbilityActivated = TryActivateAbility(InAbilityHandle, true);

		if (EndAbilityImmediately)
		{
			FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(InAbilityHandle);
			if (AbilitySpec)
			{
				UGSGameplayAbility* GSAbility = Cast<UGSGameplayAbility>(AbilitySpec->GetPrimaryInstance());
				GSAbility->ExternalEndAbility();
			}
		}

		return AbilityActivated;
	}

	return AbilityActivated;
}
```

GASShooter 对半自动和全自动枪重复使用相同批次的 `GameplayAbility`，这些枪从不直接调用 `EndAbility()`（它是由仅本地能力在能力之外处理，该能力管理玩家输入以及基于当前开火模式对批量能力的调用）。由于所有 RPC 都必须在 `FScopedServerAbilityRPCBatcher` 的范围内发生，因此我提供了 `EndAbilityImmediately` 参数，以便仅控制/管理本地功能可以指定此功能是否应批处理 `EndAbility()` 调用（半自动），或者不批处理 `EndAbility()` 调用（全自动），并且 `EndAbility()` 调用将在稍后的某个时间自行发生远程过程调用。

GASShooter 公开了一个蓝图节点以允许批处理能力，上述仅限本地的能力使用该能力来触发批处理能力。

![Activate Batched Ability](https://github.com/tranek/GASDocumentation/raw/master/Images/batchabilityactivate.png)

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-ga-netsecuritypolicy"></a>
#### 4.6.16 Net Security Policy（网络安全策略）
`GameplayAbility` 的 `NetSecurityPolicy` 确定能力应在网络上的何处执行。它可以防止客户端尝试执行受限功能。

| `NetSecurityPolicy`     | 描述                                                                                                                                        |
| ----------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------- |
| `ClientOrServer`        | 无安全要求。客户端或服务器可以自由触发该能力的执行和终止。                                           |
| `ServerOnlyExecution`   | 请求执行此功能的客户端将被服务器忽略。客户端仍然可以请求服务器取消或终止此能力。 |
| `ServerOnlyTermination` | 客户端请求取消或结束此能力将被服务器忽略。客户仍然可以请求执行该功能。      |
| `ServerOnly`            | 服务器控制该能力的执行和终止。客户端提出的任何请求都将被忽略。                                      |

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-at"></a>
### 4.7 Ability Tasks

<a name="concepts-at-definition"></a>
### 4.7.1 Ability Task 的定义
`GameplayAbilities` 仅在一帧中执行。这本身并没有提供太多的灵活性。为了执行随时间推移发生的操作或需要响应稍后某个时间点触发的委托，我们使用称为 `AbilityTasks` 的潜在操作。

GAS 附带许多开箱即用的 `AbilityTasks`：
* 使用 `RootMotionSource` 移动角色的任务
* 播放Animation Montage的任务
* 响应 `Attribute` 更改的任务
* 响应 `GameplayEffect` 更改的任务
* 响应玩家输入的任务
* 还有更多

`UAbilityTask` 构造函数强制执行硬编码的游戏范围内最多同时运行 1000 个并发 `AbilityTasks`。在为世界上同时拥有数百个角色（例如 RTS 游戏）的游戏设计 `GameplayAbilities` 时，请记住这一点。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-at-definition"></a>
### 4.7.2 自定义 Ability Tasks
通常您会创建自己的自定义 `AbilityTasks`（用 C++ 语言）。示例项目附带两个自定义 `AbilityTasks`：
1. `PlayMontageAndWaitForEvent` 是默认 `PlayMontageAndWait` 和 `WaitGameplayEvent` `AbilityTasks` 的组合。这允许Animation Montage将游戏事件从 `AnimNotifies` 发送回启动它们的 `GameplayAbility`。使用它可以在Animation Montage期间的特定时间触发动作。
1. `WaitReceiveDamage` 监听 `OwnerActor` 是否受到损坏。当英雄受到伤害时，被动装甲层 `GameplayAbility` 会移除一层装甲。

`AbilityTasks` 的组成如下：
* 创建 `AbilityTask` 新实例的静态函数
* 当 `AbilityTask` 完成其目的时广播的代表
* `Activate()` 函数用于启动其主要工作、绑定到外部委托等。
* 用于清理的 `OnDestroy()` 函数，包括它绑定到的外部委托
* 绑定到的任何外部委托的回调函数
* 成员变量和任何内部辅助函数

**注意：** `AbilityTasks` 只能声明一种类型的输出委托。所有输出委托都必须属于此类型，无论它们是否使用参数。传递未使用的委托参数的默认值。

`AbilityTasks`仅运行在运行所属`GameplayAbility`的客户端或服务器上；但是，可以通过在 `AbilityTask` 构造函数中设置 `bSimulatedTask = true;`、覆盖 `virtual void InitSimulatedTask(UGameplayTasksComponent& InGameplayTasksComponent);` 并设置要复制的任何成员变量，将 `AbilityTasks` 设置为在模拟客户端上运行。这仅在极少数情况下有用，例如运动 `AbilityTasks`，您不想复制每个运动变化，而是模拟整个运动 `AbilityTask`。所有 `RootMotionSource` `AbilityTasks` 均执行此操作。以 `AbilityTask_MoveToLocation.h/.cpp` 为例。

如果您在 `AbilityTask` 构造函数中设置 `bTickingTask = true;` 并覆盖 `virtual void TickTask(float DeltaTime);`，则 `AbilityTasks` 可以是 `Tick`。当您需要跨帧平滑地对值进行 lerp 时，这非常有用。以 `AbilityTask_MoveToLocation.h/.cpp` 为例。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-at-using"></a>
### 4.7.3 使用 Ability Tasks
要使用 C++ 创建并激活 `AbilityTask`（来自 `GDGA_FireGun.cpp`）：
```c++
UGDAT_PlayMontageAndWaitForEvent* Task = UGDAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, MontageToPlay, FGameplayTagContainer(), 1.0f, NAME_None, false, 1.0f);
Task->OnBlendOut.AddDynamic(this, &UGDGA_FireGun::OnCompleted);
Task->OnCompleted.AddDynamic(this, &UGDGA_FireGun::OnCompleted);
Task->OnInterrupted.AddDynamic(this, &UGDGA_FireGun::OnCancelled);
Task->OnCancelled.AddDynamic(this, &UGDGA_FireGun::OnCancelled);
Task->EventReceived.AddDynamic(this, &UGDGA_FireGun::EventReceived);
Task->ReadyForActivation();
```

在蓝图中，我们仅使用为 `AbilityTask` 创建的蓝图节点。我们不必调用 `ReadyForActivation()`。这是由 `Engine/Source/Editor/GameplayTasksEditor/Private/K2Node_LatentGameplayTaskCall.cpp` 自动调用的。 `K2Node_LatentGameplayTaskCall` 还会自动调用 `BeginSpawningActor()` 和 `FinishSpawningActor()`（如果它们存在于 `AbilityTask` 类中）（请参阅 `AbilityTask_WaitTargetData`）。重申一下，`K2Node_LatentGameplayTaskCall` 仅为蓝图提供自动魔法。在C++中，我们必须手动调用`ReadyForActivation()`、`BeginSpawningActor()`和`FinishSpawningActor()`。

![Blueprint WaitTargetData AbilityTask](https://github.com/tranek/GASDocumentation/raw/master/Images/abilitytask.png)

要手动取消 `AbilityTask`，只需在蓝图（称为 `Async Task Proxy`）或 C++ 中对 `AbilityTask` 对象调用 `EndTask()` 即可。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-at-rms"></a>
### 4.7.4 Root Motion Source Ability Tasks
GAS 附带 `AbilityTasks`，用于随着时间的推移移动 `Characters`，以使用挂接到 `CharacterMovementComponent` 的 `Root Motion Sources` 进行击退、复杂的跳跃、拉动和冲刺等操作。

**注意：** 预测 `RootMotionSource` `AbilityTasks` 适用于引擎版本 4.19 和 4.25+。引擎版本 4.20-4.24 的预测存在缺陷；然而，`AbilityTasks` 仍然可以在多人游戏中发挥其功能，并进行轻微的网络修正，并且在单人游戏中完美运行。可以将 4.25 中的 [预测修复](https://github.com/EpicGames/UnrealEngine/commit/94107438dd9f490e7b743f8e13da46927051bf33#diff-65f6196f9f28f560f95bd578e07e290c) 挑选到定制的 4.20-4.24 发动机中。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-gc"></a>
### 4.8 Gameplay Cues（GC）

<a name="concepts-gc-definition"></a>
#### 4.8.1 Gameplay Cue 的定义
`GameplayCues` (`GC`) 执行与游戏无关的事物，例如声音效果、粒子效果、相机抖动等。`GameplayCues` 通常会在本地进行复制（除非明确地在本地进行 `Executed`、`Added` 或 `Removed`）和预测。

我们通过 `ASC` 将带有**强制父名称 `GameplayCue.`** 和事件类型（`Execute`、`Add` 或 `Remove`）的相应 `GameplayTag` 发送到 `GameplayCueManager` 来触发 `GameplayCues`。 `GameplayCueNotify` 对象和其他实现 `IGameplayCueInterface` 的 `Actors` 可以基于 `GameplayCue's` `GameplayTag` (`GameplayCueTag`) 订阅这些事件。

**注意：** 重申一下，`GameplayCue` `GameplayTags` 需要从 `GameplayCue` 的父级 `GameplayTag` 开始。例如，有效的 `GameplayCue` `GameplayTag` 可能是 `GameplayCue.A.B.C`。

有 `GameplayCueNotifies`、`Static` 和 `Actor` 两个等级。它们响应不同的事件，不同类型的 `GameplayEffects` 可以触发它们。用您的逻辑覆盖相应的事件。

| `GameplayCue`级                                                                                                                  | 事件             | `GameplayEffect`型    | 描述                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          |
| ------------------------------------------------------------------------------------------------------------------------------------ | ----------------- | ------------------------ | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| [`GameplayCueNotify_Static`](https://docs.unrealengine.com/en-US/API/Plugins/GameplayAbilities/UGameplayCueNotify_Static/index.html) | `Execute`         | `Instant` 或 `Periodic`  | 静态 `GameplayCueNotifies` 在 `ClassDefaultObject` 上运行（意味着没有实例），非常适合一次性效果，例如命中冲击。                                                                                                                                                                                                                                                                                                                                                                        |
| [`GameplayCueNotify_Actor`](https://docs.unrealengine.com/en-US/BlueprintAPI/GameplayCueNotify/index.html)                           | `Add` 或 `Remove` | `Duration` 或 `Infinite` | 当 `Added` 时，Actor `GameplayCueNotifies` 会生成一个新实例。因为它们是实例化的，所以它们可以随着时间的推移执行操作，直到它们成为 `Removed`。这些非常适合循环声音和粒子效果，当删除背景 `Duration` 或 `Infinite` `GameplayEffect` 或手动调用删除时，这些声音和粒子效果将被删除。这些还带有选项来管理允许同时使用 `Added` 的数量，以便相同效果的多个应用程序仅启动一次声音或粒子。 |

从技术上讲，`GameplayCueNotifies` 可以响应任何事件，但这通常是我们使用它们的方式。

**注意：**使用`GameplayCueNotify_Actor`时，请勾选`Auto Destroy on Remove`，否则后续调用`Add`，`GameplayCueTag`将无法工作。

当使用除 `Full`、`Add` 和 `Remove` 之外的 `ASC` [Replication Mode（复制模式）](#concepts-asc-rm) 时，`GC` 事件将在服务器玩家（监听服务器）上触发两次 - 一次用于应用 `GE`，另一次从“最小”`NetMultiCast` 到客户端。但是，`WhileActive` 事件仍然只会触发一次。所有事件只会在客户端上触发一次。

示例项目包括用于眩晕和冲刺效果的 `GameplayCueNotify_Actor`。它还具有用于 FireGun 射弹冲击的 `GameplayCueNotify_Static`。这些 `GCs` 可以通过 [在本地触发它们](#concepts-gc-local) 进一步优化，而不是通过 `GE` 复制它们。我选择在示例项目中展示使用它们的初学者方法。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-gc-trigger"></a>
#### 4.8.2 触发 Gameplay Cues

成功应用后（未被标签或免疫阻挡）的`GameplayEffect`内部，填写所有应触发的`GameplayCues`的`GameplayTags`。

![GameplayCue Triggered from a GameplayEffect](https://github.com/tranek/GASDocumentation/raw/master/Images/gcfromge.png)

`UGameplayAbility` 向 `Execute`、`Add` 或 `Remove` `GameplayCues` 提供蓝图节点。

![GameplayCue Triggered from a GameplayAbility](https://github.com/tranek/GASDocumentation/raw/master/Images/gcfromga.png)

在 C++ 中，您可以直接在 `ASC` 上调用函数（或将它们暴露给 `ASC` 子类中的蓝图）：

```c++
/** GameplayCues can also come on their own. These take an optional effect context to pass through hit result, etc */
void ExecuteGameplayCue(const FGameplayTag GameplayCueTag, FGameplayEffectContextHandle EffectContext = FGameplayEffectContextHandle());
void ExecuteGameplayCue(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters);

/** Add a persistent gameplay cue */
void AddGameplayCue(const FGameplayTag GameplayCueTag, FGameplayEffectContextHandle EffectContext = FGameplayEffectContextHandle());
void AddGameplayCue(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters);

/** Remove a persistent gameplay cue */
void RemoveGameplayCue(const FGameplayTag GameplayCueTag);
	
/** Removes any GameplayCue added on its own, i.e. not as part of a GameplayEffect. */
void RemoveAllGameplayCues();
```

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-gc-local"></a>
#### 4.8.3 本地 Gameplay Cues
默认情况下，会复制从 `GameplayAbilities` 和 `ASC` 触发 `GameplayCues` 的公开函数。每个 `GameplayCue` 事件都是一个多播 RPC。这可能会导致大量 RPC。 GAS 还强制每个网络更新最多使用两个相同的 `GameplayCue` RPC。我们通过尽可能使用本地 `GameplayCues` 来避免这种情况。本地 `GameplayCues` 仅限单个客户端上的 `Execute`、`Add` 或 `Remove`。

本地`GameplayCues`的使用场景：
* 弹丸撞击
* 近战碰撞影响
* `GameplayCues` 从Animation Montage中发射

您应该添加到 `ASC` 子类中的本地 `GameplayCue` 函数：

```c++
UFUNCTION(BlueprintCallable, Category = "GameplayCue", Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
void ExecuteGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters);

UFUNCTION(BlueprintCallable, Category = "GameplayCue", Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
void AddGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters);

UFUNCTION(BlueprintCallable, Category = "GameplayCue", Meta = (AutoCreateRefTerm = "GameplayCueParameters", GameplayTagFilter = "GameplayCue"))
void RemoveGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters& GameplayCueParameters);
```

```c++
void UPAAbilitySystemComponent::ExecuteGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters & GameplayCueParameters)
{
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(GetOwner(), GameplayCueTag, EGameplayCueEvent::Type::Executed, GameplayCueParameters);
}

void UPAAbilitySystemComponent::AddGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters & GameplayCueParameters)
{
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(GetOwner(), GameplayCueTag, EGameplayCueEvent::Type::OnActive, GameplayCueParameters);
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(GetOwner(), GameplayCueTag, EGameplayCueEvent::Type::WhileActive, GameplayCueParameters);
}

void UPAAbilitySystemComponent::RemoveGameplayCueLocal(const FGameplayTag GameplayCueTag, const FGameplayCueParameters & GameplayCueParameters)
{
	UAbilitySystemGlobals::Get().GetGameplayCueManager()->HandleGameplayCue(GetOwner(), GameplayCueTag, EGameplayCueEvent::Type::Removed, GameplayCueParameters);
}
```

如果 `GameplayCue` 本地为 `Added`，则本地应该为 `Removed`。如果通过复制是 `Added`，则应该是通过复制 `Removed`。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-gc-parameters"></a>
#### 4.8.4 Gameplay Cue 的参数
`GameplayCues` 接收 `FGameplayCueParameters` 结构，其中包含 `GameplayCue` 的额外信息作为参数。如果您从 `GameplayAbility` 或 `ASC` 上的函数手动触发 `GameplayCue`，则必须手动填写传递到 `GameplayCue` 的 `GameplayCueParameters` 结构。如果 `GameplayCue` 由 `GameplayEffect` 触发，则以下变量将自动填充到 `GameplayCueParameters` 结构中：

* 聚合源标签
* 聚合目标标签
* GameplayEffect等级
* 能力等级
* [EffectContext](#concepts-ge-context)
* 幅度（如果 `GameplayEffect` 在 `GameplayCue` 标签容器上方的下拉列表中选择了 `Attribute` 幅度，以及影响该 `Attribute` 的相应 `Modifier`）

当手动触发 `GameplayCue` 时，`GameplayCueParameters` 结构中的 `SourceObject` 变量可能是向 `GameplayCue` 传递任意数据的好地方。

**注意：** 参数结构中的某些变量（例如 `Instigator`）可能已存在于 `EffectContext` 中。 `EffectContext` 还可以包含 `FHitResult`，用于在世界中生成 `GameplayCue` 的位置。子类化 `EffectContext` 可能是将更多数据传递给 `GameplayCues` 的好方法，尤其是那些由 `GameplayEffect` 触发的数据。

有关详细信息，请参阅 [`UAbilitySystemGlobals`](#concepts-asg) 中填充 `GameplayCueParameters` 结构的 3 个函数。它们是虚拟的，因此您可以覆盖它们以自动填充更多信息。

```c++
/** Initialize GameplayCue Parameters */
virtual void InitGameplayCueParameters(FGameplayCueParameters& CueParameters, const FGameplayEffectSpecForRPC &Spec);
virtual void InitGameplayCueParameters_GESpec(FGameplayCueParameters& CueParameters, const FGameplayEffectSpec &Spec);
virtual void InitGameplayCueParameters(FGameplayCueParameters& CueParameters, const FGameplayEffectContextHandle& EffectContext);
```

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-gc-manager"></a>
#### 4.8.5 Gameplay Cue Manager
默认情况下，`GameplayCueManager` 将扫描整个游戏目录以查找 `GameplayCueNotifies` 并将其加载到内存中。我们可以通过在`DefaultGame.ini`中设置来改变`GameplayCueManager`扫描的路径。

```
[/Script/GameplayAbilities.AbilitySystemGlobals]
GameplayCueNotifyPaths="/Game/GASDocumentation/Characters"
```

我们确实希望 `GameplayCueManager` 扫描并找到所有 `GameplayCueNotifies`；然而，我们不希望它异步加载播放中的每一个。这会将每个 `GameplayCueNotify` 及其所有引用的声音和粒子放入内存中，无论它们是否在关卡中使用。在像《Paragon》这样的大型游戏中，内存中可能会存在数百兆字节的不需要的资源，并导致启动时卡顿和游戏冻结。

在启动时异步加载每个 `GameplayCue` 的另一种方法是仅在游戏中触发时异步加载 `GameplayCues`。这可以减少不必要的内存使用和潜在的游戏硬冻结，同时异步加载每个 `GameplayCue`，以换取在游戏过程中第一次触发特定 `GameplayCue` 时可能出现的延迟效果。对于 SSD 来说，这种潜在的延迟是不存在的。我没有在硬盘上测试过。如果在 UE 编辑器中使用此选项，并且编辑器需要编译粒子系统，则在第一次加载 GameplayCues 期间可能会出现轻微的故障或冻结。这在构建中不是问题，因为粒子系统已经被编译。

首先，我们必须子类化 `UGameplayCueManager` 并告诉 `AbilitySystemGlobals` 类使用 `DefaultGame.ini` 中的 `UGameplayCueManager` 子类。

```
[/Script/GameplayAbilities.AbilitySystemGlobals]
GlobalGameplayCueManagerClass="/Script/ParagonAssets.PBGameplayCueManager"
```

在我们的 `UGameplayCueManager` 子类中，覆盖 `ShouldAsyncLoadRuntimeObjectLibraries()`。

```c++
virtual bool ShouldAsyncLoadRuntimeObjectLibraries() const override
{
	return false;
}
```

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-gc-prevention"></a>
#### 4.8.6 阻止触发 Gameplay Cues
有时我们不希望 `GameplayCues` 被触发。例如，如果我们阻止攻击，我们可能不想播放附加到伤害 `GameplayEffect` 的命中冲击力，或者改为播放自定义冲击力。我们可以在 [`GameplayEffectExecutionCalculations`](#concepts-ge-ec) 内部执行此操作，方法是调用 `OutExecutionOutput.MarkGameplayCuesHandledManually()`，然后手动将 `GameplayCue` 事件发送到 `Target` 或 `Source's` `ASC`。

如果您不想让任何 `GameplayCues` 在特定的 `ASC` 上触发，您可以设置 `AbilitySystemComponent->bSuppressGameplayCues = true;`。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-gc-batching"></a>
#### 4.8.7 Gameplay Cue 批处理
每个 `GameplayCue` 触发的都是一个不可靠的 NetMulticast RPC。在我们同时触发多个 `GCs` 的情况下，有一些优化方法可以将它们压缩为一个 RPC 或通过发送更少的数据来节省带宽。

<a name="concepts-gc-batching-manualrpc"></a>
##### 4.8.7.1 手动调用 RPC
假设您有一把可以发射八颗子弹的霰弹枪。这是八个痕迹和影响 `GameplayCues`。 [GASShooter](https://github.com/tranek/GASShooter) 采用一种懒惰的方法，将所有跟踪信息作为 [`TargetData`](#concepts-targeting-data) 存储到 [`EffectContext`](#concepts-ge-ec) 中，将它们组合成一个 RPC。虽然这将 RPC 从 8 个减少到 1 个，但它仍然在该 RPC 中通过网络发送大量数据（约 500 字节）。更优化的方法是发送带有自定义结构的 RPC，您可以在其中有效地对命中位置进行编码，或者可以为其提供随机种子数以在接收端重新创建/近似影响位置。然后，客户端将解压此自定义结构并返回到 [locally executed `GameplayCues`](#concepts-gc-local)。

这是如何运作的：
1. 声明 `FScopedGameplayCueSendContext`。这会抑制 `UGameplayCueManager::FlushPendingCues()` 直到它超出范围，这意味着所有 `GameplayCues` 将排队直到 `FScopedGameplayCueSendContext` 超出范围。
1. 覆盖`UGameplayCueManager::FlushPendingCues()`合并`GameplayCues`可以根据一些自定义批量在一起`GameplayTag`到您的自定义结构中并将其 RPC 到客户端。
1. 客户端接收自定义结构并将其解压到本地执行的 `GameplayCues` 中。

当您需要 `GameplayCues` 的特定参数与 `GameplayCueParameters` 提供的内容不符并且您不想将它们添加到 `EffectContext` 时，也可以使用此方法，例如伤害数字、暴击指示器、破碎护盾指示器、致命一击指示器等。

https://forums.unrealengine.com/development-discussion/c-gameplay-programming/1711546-fscopedgameplaycuesendcontext-gameplaycuemanager

<a name="concepts-gc-batching-gcsonge"></a>
##### 4.8.7.2 一个 GE 上的多个 GC
`GameplayEffect` 上的所有 `GameplayCues` 已在一个 RPC 中发送。默认情况下，`UGameplayCueManager::InvokeGameplayCueAddedAndWhileActive_FromSpec()` 将在不可靠 NetMulticast 中发送整个 `GameplayEffectSpec`（但转换为 `FGameplayEffectSpecForRPC`），无论 `ASC` 的 `Replication Mode` 是什么。这可能会占用大量带宽，具体取决于 `GameplayEffectSpec` 中的内容。我们可以通过设置 cvar `AbilitySystem.AlwaysConvertGESpecToGCParams 1` 来优化这一点。这会将 `GameplayEffectSpecs` 转换为 `FGameplayCueParameter` 结构，并对这些结构进行 RPC，而不是整个 `FGameplayEffectSpecForRPC`。这可能会节省带宽，但信息也会减少，具体取决于 `GESpec` 如何转换为 `GameplayCueParameters` 以及您的 `GCs` 需要了解什么。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-gc-events"></a>
#### 4.8.8 Gameplay Cue 的事件
`GameplayCues` 响应具体`EGameplayCueEvents`：

| `EGameplayCueEvent` | 描述                                                                                                                                                                                                                                                                                                                         |
| ------------------- | ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `OnActive`          | 当 `GameplayCue` 被激活（添加）时调用。                                                                                                                                                                                                                                                                                   |
| `WhileActive`       | 当 `GameplayCue` 处于活动状态时调用，即使它实际上并未刚刚应用（正在进行加入等）。这不是`Tick`！当添加 `GameplayCueNotify_Actor` 或变得相关时，它会像 `OnActive` 一样被调用一次。如果您需要`Tick()`，只需使用`GameplayCueNotify_Actor`的`Tick()`即可。毕竟是`AActor`。 |
| `Removed`           | 当 `GameplayCue` 被移除时调用。响应该事件的蓝图`GameplayCue`函数是`OnRemove`。                                                                                                                                                                                                             |
| `Executed`          | 执行 `GameplayCue` 时调用：即时效果或周期性 `Tick()`。响应该事件的蓝图 `GameplayCue` 函数是 `OnExecute`。                                                                                                                                                                     |

使用 `OnActive` 来处理 `GameplayCue` 中在 `GameplayCue` 开始时发生的任何事情，但如果迟到的加入者错过了也没关系。使用 `WhileActive` 在 `GameplayCue` 中实现您希望后来加入的人看到的持续效果。例如，如果 MOBA 爆炸中的塔结构有 `GameplayCue`，则您可以将初始爆炸粒子系统和爆炸声音放入 `OnActive`，并将任何残留的持续火焰粒子或声音放入 `WhileActive`。在这种情况下，迟到的加入者重播 `OnActive` 的初始爆炸是没有意义的，但您希望他们看到 `WhileActive` 爆炸发生后地面上持续的循环火焰效果。 `OnRemove` 应清除 `OnActive` 和 `WhileActive` 中添加的任何内容。每次 Actor 进入 `GameplayCueNotify_Actor` 的相关范围时，都会调用 `WhileActive`。每次 Actor 离开 `GameplayCueNotify_Actor` 的相关范围时，都会调用 `OnRemove`。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-gc-reliability"></a>
#### 4.8.9 Gameplay Cue 的可靠性

`GameplayCues` 一般来说应该被认为是不可靠的，因此不适合任何直接影响游戏玩法的东西。

**执行的`GameplayCues`：**这些`GameplayCues`是通过不可靠的多播应用的，并且总是不可靠的。

**`GameplayCues` 从 `GameplayEffects` 应用：**
* 自治代理可靠地接收 `OnActive`、`WhileActive` 和 `OnRemove`  
`FActiveGameplayEffectsContainer::NetDeltaSerialize()` 调用 `UAbilitySystemComponent::HandleDeferredGameplayCues()` 来调用 `OnActive` 和 `WhileActive`。 `FActiveGameplayEffectsContainer::RemoveActiveGameplayEffectGrantedTagsAndModifiers()` 呼叫 `OnRemoved`。
* 模拟代理可靠地接收 `WhileActive` 和 `OnRemove`  
`UAbilitySystemComponent::MinimalReplicationGameplayCues` 的复制调用 `WhileActive` 和 `OnRemove`。 `OnActive` 事件由不可靠的多播调用。

**`GameplayCues` 在没有 `GameplayEffect` 的情况下应用：**
* 自治代理可靠地接收`OnRemove`  
`OnActive` 和 `WhileActive` 事件由不可靠多播调用。
* 模拟代理可靠地接收 `WhileActive` 和 `OnRemove`  
`UAbilitySystemComponent::MinimalReplicationGameplayCues` 的复制调用 `WhileActive` 和 `OnRemove`。 `OnActive` 事件由不可靠的多播调用。

如果您需要 `GameplayCue` 中的某些内容“可靠”，请从 `GameplayEffect` 中应用它，并使用 `WhileActive` 添加 FX 和 `OnRemove` 删除 FX。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-asg"></a>
### 4.9 Ability System Globals
[`AbilitySystemGlobals`](https://docs.unrealengine.com/en-US/API/Plugins/GameplayAbilities/UAbilitySystemGlobals/index.html) 类保存有关 GAS 的全局信息。大多数变量可以从 `DefaultGame.ini` 进行设置。一般来说，您不必与此类交互，但您应该知道它的存在。如果您需要对 [`GameplayCueManager`](#concepts-gc-manager) 或 [`GameplayEffectContext`](#concepts-ge-context) 等内容进行子类化，则必须通过 `AbilitySystemGlobals` 来完成。

要子类化 `AbilitySystemGlobals`，请在 `DefaultGame.ini` 中设置类名：
```
[/Script/GameplayAbilities.AbilitySystemGlobals]
AbilitySystemGlobalsClassName="/Script/ParagonAssets.PAAbilitySystemGlobals"
```

<a name="concepts-asg-initglobaldata"></a>
#### 4.9.1 InitGlobalData()
在UE 4.24和5.2之间，需要调用`UAbilitySystemGlobals::Get().InitGlobalData()`才能使用[`TargetData`](#concepts-targeting-data)，否则您将收到与`ScriptStructCache`相关的错误，并且客户端将与服务器断开连接。该函数在项目中只需要调用一次。 Fortnite 从 `UAssetManager::StartInitialLoading()` 调用它，Paragon 从 `UEngine::Init()` 调用它。我发现将其放在 `UAssetManager::StartInitialLoading()` 中是一个好地方，如示例项目所示。我会考虑将此样板代码复制到您的项目中，以避免 `TargetData` 出现问题。从 5.3 开始，它会被自动调用。

如果您在使用 `AbilitySystemGlobals` `GlobalAttributeSetDefaultsTableNames` 时遇到崩溃，您可能需要稍后调用 `UAbilitySystemGlobals::Get().InitGlobalData()`，就像 Fortnite 中的 `AssetManager` 或 `GameInstance` 中一样。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-p"></a>
### 4.10 预测
GAS 开箱即用，支持客户端预测；然而，它并不能预测一切。 GAS中的客户端预测意味着客户端不必等待服务器的许可来激活`GameplayAbility`并应用`GameplayEffects`。它可以“预测”授予其执行此操作权限的服务器，并预测它将应用 `GameplayEffects` 的目标。然后，服务器在客户端激活后运行 `GameplayAbility` 网络延迟时间，并告诉客户端他的预测是否正确。如果客户端的任何预测是错误的，他将“回滚”他的“错误预测”中的更改以匹配服务器。

GAS 相关预测的权威来源是插件源代码中的 `GameplayPrediction.h`。

Epic 的心态是只预测你“能逃脱惩罚”的事情。例如，Paragon 和 Fortnite 不会预测伤害。他们很可能使用 [`ExecutionCalculations`](#concepts-ge-ec) 来造成无法预测的伤害。这并不是说您不能尝试预测某些事情，例如损坏。无论如何，如果你这样做并且它对你来说效果很好，那就太好了。

> ...我们也并不完全致力于“预测一切：无缝且自动”的解决方案。我们仍然认为玩家的预测最好保持在最低限度（意思是：预测你能逃脱的最小数量的东西）。

*Epic 的 Dave Ratti 对新 [Network Prediction 插件](#concepts-p-npp) 的评论*

**预测是什么：**
> * 能力激活
> *	触发事件
> *	GameplayEffect应用：
>    * 属性修改（例外：执行当前不预测，仅属性修改）
>    * Gameplay Tag 的修改
> * GameplayCue事件（来自预测GameplayEffect及其本身）
> * Montage
> * 运动（内置于UE的UCharacterMovement中）

**未预测的内容：**
> * GameplayEffect移除
> * GameplayEffect 周期性效果（点滴答）

*来自 `GameplayPrediction.h`*

虽然我们可以预测 `GameplayEffect` 的应用，但我们无法预测 `GameplayEffect` 的移除。解决此限制的一种方法是预测当我们想要删除 `GameplayEffect` 时的相反效果。假设我们预测移动速度减慢 40%。我们可以通过应用 40% 的移动速度增益来预测性地移除它。然后同时删除两个`GameplayEffects`。这并不适合所有场景，仍然需要支持预测 `GameplayEffect` 删除。 Epic 的 Dave Ratti 表示希望将其添加到 [GAS 的未来迭代](https://epicgames.ent.box.com/s/m1egifkxv3he3u3xezb9hzbgroxyhx89) 中。

因为我们无法预测 `GameplayEffects` 的移除，所以我们无法完全预测 `GameplayAbility` 的冷却时间，并且没有针对它们的反向 `GameplayEffect` 解决方法。服务器的复制 `Cooldown GE` 将存在于客户端上，任何绕过此的尝试（例如使用 `Minimal` 复制模式）都将被服务器拒绝。这意味着具有较高延迟的客户端需要更长的时间来告诉服务器进行冷却并接收服务器 `Cooldown GE` 的删除。这意味着延迟较高的玩家的射速会比延迟较低的玩家低，从而使他们在与延迟较低的玩家竞争时处于劣势。 Fortnite 通过使用自定义簿记而不是 `Cooldown GEs` 来避免此问题。

关于预测损坏，我个人并不推荐它，尽管它是大多数人在开始使用 GAS 时首先尝试的事情之一。我特别不建议尝试预测死亡。虽然您可以预测损坏，但这样做很棘手。如果你错误地预测了施加的伤害，玩家会看到敌人的生命值跳回。如果你试图预测死亡，这可能会特别尴尬和令人沮丧。假设你错误地预测了 `Character's` 的死亡，它开始布娃娃，但当服务器纠正它时，它会停止布娃娃并继续向你射击。

**注意：** `Instant` `GameplayEffects`（如`Cost GEs`）的变化`Attributes`可以在您身上无缝预测，预测`Instant` `Attribute`对其他角色的变化将在其`Attributes`中显示短暂的异常或“blip”。预测的 `Instant` `GameplayEffects` 实际上会像 `Infinite` `GameplayEffects` 一样处理，这样如果预测错误，它们可以回滚。当应用服务器的 `GameplayEffect` 时，可能存在两个相同的 `GameplayEffect's`，导致 `Modifier` 被应用两次或在短时间内根本不应用。它最终会自行纠正，但有时玩家会注意到这一点。

GAS的预测实现试图解决的问题：
> 1. “我可以这样做吗？”预测的基本协议。
> 2. “撤消”当预测失败时如何撤消副作用。
> 3. “重做”如何避免重播我们在本地预测但也会从服务器复制的副作用。
> 4. “完整性”如何确保我们/真正/预测了所有副作用。
> 5. “依赖关系”如何管理依赖预测和预测事件链。
> 6. “覆盖”如何预测性地覆盖由服务器复制/拥有的状态。

*来自 `GameplayPrediction.h`*

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-p-key"></a>
#### 4.10.1 Prediction Key
GAS 的预测基于 `Prediction Key` 的概念，它是客户端在激活 `GameplayAbility` 时生成的整数标识符。

* 客户端在激活 `GameplayAbility` 时生成预测密钥。这是 `Activation Prediction Key`。
* 客户端将此预测密钥以 `CallServerTryActivateAbility()` 发送到服务器。
* 当预测密钥有效时，客户端将此预测密钥添加到其应用的所有 `GameplayEffects` 中。
* 客户的预测密钥超出范围。进一步预测相同`GameplayAbility`的效果需要新的[范围预测窗口](#concepts-p-windows)。


* 服务器从客户端接收预测密钥。
* 服务器将此预测密钥添加到它应用的所有 `GameplayEffects` 中。
* 服务器将预测密钥复制回客户端。


* 客户端从服务器接收复制的 `GameplayEffects` 以及用于应用它们的预测密钥。如果任何复制的 `GameplayEffects` 与客户端使用相同预测密钥应用的 `GameplayEffects` 相匹配，则它们被正确预测。目标上将暂时有两份 `GameplayEffect` 的副本，直到客户端删除其预测的一份。
* 客户端接收从服务器返回的预测密钥。这是 `Replicated Prediction Key`。该预测密钥现已标记为过时。
* 客户端删除它使用现已过时的复制预测密钥创建的**所有** `GameplayEffects`。服务器复制的 `GameplayEffects` 将持续存在。客户端添加但未从服务器收到匹配的复制版本的任何 `GameplayEffects` 均被错误预测。

在 `GameplayAbilities` 中从激活预测密钥开始的 `Activation` 指令“窗口”的原子分组期间，保证预测密钥有效。您可以认为这仅在一帧内有效。来自潜在操作 `AbilityTasks` 的任何回调将不再具有有效的预测密钥，除非 `AbilityTask` 具有生成新 [范围预测窗口](#concepts-p-windows) 的内置同步点。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-p-windows"></a>
#### 4.10.2 在 Ability 中创建新的预测窗口
要在 `AbilityTasks` 的回调中继续预测操作，需要使用新的 scoped prediction key 创建新的局部预测窗口。这有时称为客户端与服务器之间的同步点。部分 `AbilityTasks`（例如所有与输入有关的任务）内置了创建新预测窗口的逻辑，因此 `AbilityTasks'` 回调中的原子操作有可用的 scoped prediction key。其他任务（如 `WaitDelay`）没有为回调创建新预测窗口的内置代码。如果需要在这类 `AbilityTask`（例如 `WaitDelay`）之后预测操作，就必须使用 `WaitNetSync` `AbilityTask`，并选择 `OnlyServerWait`。当客户端执行到使用 `OnlyServerWait` 的 `WaitNetSync` 时，它会根据 `GameplayAbility's` activation prediction key 生成新的 scoped prediction key，通过 RPC 发给服务器，并附加到新应用的 `GameplayEffects` 上。当服务器执行到使用 `OnlyServerWait` 的 `WaitNetSync` 时，会等待客户端的新 scoped prediction key 到达再继续。它的作用与 activation prediction key 相同：附加到 `GameplayEffects` 上，并复制回客户端，使客户端将其标记为过期。这个 key 只在当前作用域内有效；离开作用域就意味着预测窗口关闭。同样，只有原子操作（不包含 latent 等待操作）才能使用这个新的 scoped prediction key。

您可以根据需要创建任意数量的范围预测窗口。

如果您想将同步点功能添加到您自己的自定义 `AbilityTasks`，请查看输入功能如何将 `WaitNetSync` `AbilityTask` 代码注入其中。

**注意：** 当使用 `WaitNetSync` 时，这会阻止服务器的 `GameplayAbility` 继续执行，直到收到客户端的消息。这可能会被恶意用户滥用，他们会破解游戏并故意延迟发送新的范围预测密钥。虽然 Epic 很少使用 `WaitNetSync`，但如果您担心的话，建议您可能构建一个新版本的 `AbilityTask`，其延迟会在没有客户端的情况下自动继续。

示例项目在 Sprint `GameplayAbility` 中使用 `WaitNetSync` 在每次应用耐力成本时创建一个新的范围预测窗口，以便我们可以对其进行预测。理想情况下，我们在应用成本和冷却时间时需要一个有效的预测密钥。

如果您有一个预测的 `GameplayEffect` 在拥有的客户端上播放了两次，则您的预测密钥已过时，并且您遇到了“重做”问题。通常，您可以通过在应用 `GameplayEffect` 创建新的作用域预测密钥之前放置 `WaitNetSync` `AbilityTask` 和 `OnlyServerWait` 来解决此问题。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-p-spawn"></a>
#### 4.10.3 预测生成 Actors
在客户端上预测性地生成 `Actors` 是一个高级主题。 GAS 不提供开箱即用的处理此问题的功能（`SpawnActor` `AbilityTask` 仅在服务器上生成 `Actor`）。关键概念是在客户端和服务器上生成复制的 `Actor`。

如果 `Actor` 只是装饰性的或不用于任何游戏目的，则简单的解决方案是覆盖 `Actor's` `IsNetRelevantFor()` 函数以限制服务器复制到拥有的客户端。拥有的客户端将拥有其本地生成的版本，而服务器和其他客户端将拥有服务器的复制版本。
```c++
bool APAReplicatedActorExceptOwner::IsNetRelevantFor(const AActor * RealViewer, const AActor * ViewTarget, const FVector & SrcLocation) const
{
	return !IsOwnedBy(ViewTarget);
}
```

如果生成的 `Actor` 像需要预测伤害的射弹一样影响游戏玩法，那么您需要超出本文档范围的高级逻辑。在 Epic Games 的 GitHub 上查看 UnrealTournament 如何预测性地生成射弹。他们仅在拥有的客户端上生成一个虚拟射弹，该虚拟射弹与服务器的复制射弹同步。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-p-future"></a>
#### 4.10.4 GAS 预测机制的未来
`GameplayPrediction.h` 表示，未来他们可能会添加预测 `GameplayEffect` 移除和定期 `GameplayEffects` 的功能。

Epic 的 Dave Ratti 让 [表示有兴趣](https://epicgames.ent.box.com/s/m1egifkxv3he3u3xezb9hzbgroxyhx89) 修复了预测冷却时间的 `latency reconciliation` 问题，这使得延迟较高的玩家相对于延迟较低的玩家处于不利地位。

Epic 的新 [`Network Prediction` plugin](#concepts-p-npp) 预计将与 GAS 完全互操作，就像之前的 `CharacterMovementComponent` 一样。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-p-npp"></a>
#### 4.10.5 Network Prediction 插件
Epic 最近启动了一项计划，用新的 `Network Prediction` 插件替换 `CharacterMovementComponent`。该插件仍处于早期阶段，但可以在虚幻引擎 GitHub 上尽早访问。现在判断该引擎将在哪个未来版本中首次亮相还为时过早。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-targeting"></a>
### 4.11 目标选择

<a name="concepts-targeting-data"></a>
#### 4.11.1 Target Data
[`FGameplayAbilityTargetData`](https://docs.unrealengine.com/en-US/API/Plugins/GameplayAbilities/Abilities/FGameplayAbilityTargetData/index.html) 是一种通用结构，用于定位要通过网络传递的数据。 `TargetData` 通常会保存 `AActor`/`UObject` 参考、`FHitResults` 和其他通用位置/方向/原点信息。但是，您可以对其进行子类化，以将任何您想要的内容放入其中，作为 [pass data between the client and server in `GameplayAbilities`](#concepts-ga-data) 的简单方法。基本结构 `FGameplayAbilityTargetData` 不应直接使用，而是要进行子类化。 `GAS` 附带了一些开箱即用的子类 `FGameplayAbilityTargetData` 结构，位于 `GameplayAbilityTargetTypes.h` 中。

`TargetData` 通常由 [`Target Actors`](#concepts-targeting-actors) 生成或**手动创建**并由 [`AbilityTasks`](#concepts-at) 和 [`GameplayEffects`](#concepts-ge) 通过 [`EffectContext`](#concepts-ge-context) 消耗。由于位于`EffectContext`、[`Executions`](#concepts-ge-ec)、[`MMCs`](#concepts-ge-mmc)、[`GameplayCues`](#concepts-gc)以及[`AttributeSet`](#concepts-as)后端的函数可以访问`TargetData`。

我们通常不会直接传递 `FGameplayAbilityTargetData`，而是使用 [`FGameplayAbilityTargetDataHandle`](https://docs.unrealengine.com/en-US/API/Plugins/GameplayAbilities/Abilities/FGameplayAbilityTargetDataHandle/index.html)，它具有指向 `FGameplayAbilityTargetData` 的指针的内部 TArray。该中间结构为 `TargetData` 的多态性提供支持。

从`FGameplayAbilityTargetData`继承的例子：
```c++
USTRUCT(BlueprintType)
struct MYGAME_API FGameplayAbilityTargetData_CustomData : public FGameplayAbilityTargetData
{
    GENERATED_BODY()
public:

    FGameplayAbilityTargetData_CustomData()
    { }

    UPROPERTY()
    FName CoolName = NAME_None;

    UPROPERTY()
    FPredictionKey MyCoolPredictionKey;

    // This is required for all child structs of FGameplayAbilityTargetData
    virtual UScriptStruct* GetScriptStruct() const override
    {
    	return FGameplayAbilityTargetData_CustomData::StaticStruct();
    }

	// This is required for all child structs of FGameplayAbilityTargetData
    bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
    {
	    // The engine already defined NetSerialize for FName & FPredictionKey, thanks Epic!
        CoolName.NetSerialize(Ar, Map, bOutSuccess);
        MyCoolPredictionKey.NetSerialize(Ar, Map, bOutSuccess);
        bOutSuccess = true;
        return true;
    }
}

template<>
struct TStructOpsTypeTraits<FGameplayAbilityTargetData_CustomData> : public TStructOpsTypeTraitsBase2<FGameplayAbilityTargetData_CustomData>
{
	enum
	{
        WithNetSerializer = true // This is REQUIRED for FGameplayAbilityTargetDataHandle net serialization to work
	};
};
```
将目标数据添加到句柄：
```c++
UFUNCTION(BlueprintPure)
FGameplayAbilityTargetDataHandle MakeTargetDataFromCustomName(const FName CustomName)
{
	// Create our target data type, 
	// Handle's automatically cleanup and delete this data when the handle is destructed, 
	// if you don't add this to a handle then be careful because this deals with memory management and memory leaks so its safe to just always add it to a handle at some point in the frame!
	FGameplayAbilityTargetData_CustomData* MyCustomData = new FGameplayAbilityTargetData_CustomData();
	// Setup the struct's information to use the inputted name and any other changes we may want to do
	MyCustomData->CoolName = CustomName;
	
	// Make our handle wrapper for Blueprint usage
	FGameplayAbilityTargetDataHandle Handle;
	// Add the target data to our handle
	Handle.Add(MyCustomData);
	// Output our handle to Blueprint
	return Handle
}
```

为了获取值，需要进行类型安全检查，因为从句柄的目标数据获取值的唯一方法是使用通用 C/C++ 转换，这不是类型安全的，可能会导致对象切片和崩溃。对于类型检查，有多种方法可以执行此操作（但是您诚实地想要），两种常见的方法是：
- GameplayTag：您可以使用子类层次结构，在该子类层次结构中，您知道每当某个代码架构的功能发生时，您都可以转换为基本父类型并获取其GameplayTag，然后与继承类的转换标签进行比较。
- 脚本结构和静态结构：您可以直接进行类比较（这可能涉及大量 IF 语句或创建一些模板函数），下面是这样做的示例，但基本上您可以从任何 `FGameplayAbilityTargetData` 获取脚本结构（这是一个很好的优点，它是 `USTRUCT` 并要求任何继承类指定 `GetScriptStruct` 中的结构类型）并比较它是否是您的类型寻找。下面是使用这些函数进行类型检查的示例：
```c++
UFUNCTION(BlueprintPure)
FName GetCoolNameFromTargetData(const FGameplayAbilityTargetDataHandle& Handle, const int Index)
{   
    // NOTE, there is two versions of this '::Get(int32 Index)' function; 
    // 1) const version that returns 'const FGameplayAbilityTargetData*', good for reading target data values 
    // 2) non-const version that returns 'FGameplayAbilityTargetData*', good for modifying target data values
    FGameplayAbilityTargetData* Data = Handle.Get(Index); // This will valid check the index for you 
    
    // Valid check we have something to use, null data means nothing to cast for
    if(Data == nullptr)
    {
       	return NAME_None;
    }
    // This is basically the type checking pass, static_cast does not have type safety, this is why we do this check.
    // If we don't do this then it will object slice the struct and thus we have no way of making sure its that type.
    if(Data->GetScriptStruct() == FGameplayAbilityTargetData_CustomData::StaticStruct())
    {
        // Here is when you would do the cast because we know its the correct type already
        FGameplayAbilityTargetData_CustomData* CustomData = static_cast<FGameplayAbilityTargetData_CustomData*>(Data);    
        return CustomData->CoolName;
    }
    return NAME_None;
}
```

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-targeting-actors"></a>
#### 4.11.2 Target Actors
`GameplayAbilities` 与 `WaitTargetData` `AbilityTask` 生成 [`TargetActors`](https://docs.unrealengine.com/en-US/API/Plugins/GameplayAbilities/Abilities/AGameplayAbilityTargetActor/index.html)，以可视化和捕获来自世界的目标信息。 `TargetActors` 可以选择使用 [`GameplayAbilityWorldReticles`](#concepts-targeting-reticles) 显示当前目标。确认后，目标信息作为 [`TargetData`](#concepts-targeting-data) 返回，然后可以传递到 `GameplayEffects`。
 
`TargetActors` 基于 `AActor`，因此它们可以具有任何类型的可见组件来表示它们的目标**位置**和**方式**，例如静态网格物体或贴花。静态网格物体可用于可视化角色将构建的对象的放置。贴花可用于显示地面上的影响区域。示例项目使用 [`AGameplayAbilityTargetActor_GroundTrace`](https://docs.unrealengine.com/en-US/API/Plugins/GameplayAbilities/Abilities/AGameplayAbilityTargetActor_Grou-/index.html) 并在地面上贴花来表示流星能力的伤害区域。他们也不需要显示任何东西。例如，对于 [GASShooter](https://github.com/tranek/GASShooter) 中使用的即时追踪到目标的扫描枪来说，显示任何内容是没有意义的。

它们使用基本轨迹或碰撞重叠捕获目标信息，并根据 `TargetActor` 实现将结果从 `FHitResults` 或 `AActor` 数组转换为 `TargetData`。 `WaitTargetData` `AbilityTask` 通过其 `TEnumAsByte<EGameplayTargetingConfirmation::Type> ConfirmationType` 参数确定何时确认目标。当**不**使用`TEnumAsByte<EGameplayTargetingConfirmation::Type::Instant`时，`TargetActor`通常在`Tick()`上执行跟踪/重叠，并根据其实现将其位置更新到`FHitResult`。虽然这会在 `Tick()` 上执行跟踪/重叠，但它通常并不可怕，因为它不会被复制，并且您通常不会同时运行多个（尽管可以有多个）`TargetActor`。请注意，它使用 `Tick()` 和一些复杂的 `TargetActors` 可能会做很多事情，就像 GASShooter 中火箭发射器的辅助能力一样。虽然 `Tick()` 上的跟踪对客户端的响应非常灵敏，但如果性能损失太大，您可以考虑降低 `TargetActor` 上的滴答率。对于 `TEnumAsByte<EGameplayTargetingConfirmation::Type::Instant`，`TargetActor` 会立即生成、生成 `TargetData`，然后销毁。 `Tick()` 从未被调用。 

| `EGameplayTargetingConfirmation::Type` | 当目标确定后                                                                                                                                                                                                                                                                                                                                     |
| -------------------------------------- | -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `Instant`                              | 定位立即发生，无需特殊逻辑或用户输入来决定何时“触发”。                                                                                                                                                                                                                                                                   |
| `UserConfirmed`                        | 当用户通过 [ability is bound to a `Confirm` input](#concepts-ga-input) 或调用 `UAbilitySystemComponent::TargetConfirm()` 确认定位时，就会发生定位。 `TargetActor` 还将响应绑定的 `Cancel` 输入或调用 `UAbilitySystemComponent::TargetCancel()` 以取消定位。                              |
| `Custom`                               | GameplayTargeting Skill 负责通过调用 `UGameplayAbility::ConfirmTaskByInstanceName()` 来决定目标数据何时准备就绪。 `TargetActor` 还将响应 `UGameplayAbility::CancelTaskByInstanceName()` 以取消定位。                                                                                              |
| `CustomMulti`                          | GameplayTargeting Skill 负责通过调用 `UGameplayAbility::ConfirmTaskByInstanceName()` 来决定目标数据何时准备就绪。 `TargetActor` 还将响应 `UGameplayAbility::CancelTaskByInstanceName()` 以取消定位。不应在数据生成时结束 `AbilityTask`。                                       |

并非每个 `TargetActor` 都支持每个 EGameplayTargetingConfirmation::Type。例如，`AGameplayAbilityTargetActor_GroundTrace`不支持`Instant`确认。

`WaitTargetData` `AbilityTask` 接受一个 `AGameplayAbilityTargetActor` 类作为参数，每次激活该 `AbilityTask` 时都会生成实例，并在 `AbilityTask` 结束时销毁 `TargetActor`。`WaitTargetDataUsingActor` `AbilityTask` 接受已经生成的 `TargetActor`，但仍会在 `AbilityTask` 结束时销毁它。这两个 `AbilityTasks` 的效率不高，因为每次使用时都要生成或提供一个新生成的 `TargetActor`。它们适合制作原型；正式项目如果需要频繁生成 `TargetData`（如自动步枪），则可以考虑优化。GASShooter 提供了自定义的 [`AGameplayAbilityTargetActor`](https://github.com/tranek/GASShooter/blob/master/Source/GASShooter/Public/Characters/Abilities/GSGATA_Trace.h) 子类，以及从头实现的 [`WaitTargetDataWithReusableActor`](https://github.com/tranek/GASShooter/blob/master/Source/GASShooter/Public/Characters/Abilities/AbilityTasks/GSAT_WaitTargetDataUsingActor.h) `AbilityTask`，让您能够复用 `TargetActor`，而不必销毁它。

`TargetActors`默认不复制；但是，如果这在您的游戏中有意义，则可以让它们进行复制，以向其他玩家显示本地玩家的目标位置。它们确实包含通过 `WaitTargetData` `AbilityTask` 上的 RPC 与服务器进行通信的默认功能。如果`TargetActor`的`ShouldProduceTargetDataOnServer`属性设置为`false`，那么客户端将通过`UAbilityTask_WaitTargetData::OnTargetDataReadyCallback()`中的`CallServerSetReplicatedTargetData()`将其`TargetData` RPC到服务器确认。如果`ShouldProduceTargetDataOnServer`是`true`，客户端将向`UAbilityTask_WaitTargetData::OnTargetDataReadyCallback()`中的服务器发送通用确认事件`EAbilityGenericReplicatedEvent::GenericConfirm`、RPC，服务器将在收到RPC后进行跟踪或重叠检查，以在服务器上产生数据。如果客户端取消定位，它将向 `UAbilityTask_WaitTargetData::OnTargetDataCancelledCallback` 中的服务器发送通用取消事件 `EAbilityGenericReplicatedEvent::GenericCancel`、RPC。正如您所看到的，`TargetActor` 和 `WaitTargetData` `AbilityTask` 上都有很多代表。 `TargetActor` 响应输入以生成和广播 `TargetData` 就绪、确认或取消代表。 `WaitTargetData` 侦听 `TargetActor` 的 `TargetData` 就绪、确认和取消委托，并将该信息中继回 `GameplayAbility` 和服务器。如果您将 `TargetData` 发送到服务器，您可能需要在服务器上进行验证，以确保 `TargetData` 看起来合理，以防止作弊。直接在服务器上生成 `TargetData` 可以完全避免此问题，但可能会导致拥有客户端的错误预测。

根据您使用的 `AGameplayAbilityTargetActor` 的特定子类，不同的 `ExposeOnSpawn` 参数将在 `WaitTargetData` `AbilityTask` 节点上公开。一些常见参数包括：

| `TargetActor`常用参数 | 定义                                                                                                                                                                                                                                                                                                               |
| ------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| 调试                           | 如果是 `true`，则每当 `TargetActor` 在非发布版本中执行跟踪时，它将绘制调试跟踪/重叠信息。请记住，非 `Instant` `TargetActors` 将在 `Tick()` 上执行跟踪，因此这些调试绘制调用也将在 `Tick()` 上发生。                                                        |
| 筛选                          | [可选] 一个特殊结构，用于在发生跟踪/重叠时从目标中过滤（删除）`Actors`。典型的用例是过滤掉玩家的 `Pawn`，要求目标属于特定类别。有关更高级的用例，请参阅 [Target Data Filters](#concepts-target-data-filters)。 |
| 十字线类                   | [可选] `TargetActor` 将生成的 `AGameplayAbilityWorldReticle` 的子类。                                                                                                                                                                                                                                 |
| 十字线参数              | [可选] 配置您的标线。参见 [Reticles](#concepts-targeting-reticles)。                                                                                                                                                                                                                                        |
| 起始位置                  | 跟踪应该从哪里开始的特殊结构。通常，这将是玩家的视点、武器枪口或 `Pawn` 的位置。                                                                                                                                                                          |

对于默认的 `TargetActor` 类，`Actors` 仅当它们直接位于跟踪/重叠中时才是有效目标。如果它们留下痕迹/重叠（它们移动或您移开视线），它们就不再有效。如果您希望 `TargetActor` 记住最后一个有效目标，则需要将此功能添加到自定义 `TargetActor` 类中。我将这些称为持久目标，因为它们将持续存在，直到 `TargetActor` 收到确认或取消、`TargetActor` 在其跟踪/重叠中找到新的有效目标，或者目标不再有效（已销毁）。 GASShooter 使用持久目标作为其火箭发射器的辅助能力的寻的火箭瞄准。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-target-data-filters"></a>
#### 4.11.3 Target Data Filters
使用 `Make GameplayTargetDataFilter` 和 `Make Filter Handle` 节点，您可以过滤掉玩家的 `Pawn` 或仅选择特定类别。如果需要更高级的过滤，可以子类化 `FGameplayTargetDataFilter` 并覆盖 `FilterPassesForActor` 函数。 
```c++
USTRUCT(BlueprintType)
struct GASDOCUMENTATION_API FGDNameTargetDataFilter : public FGameplayTargetDataFilter
{
	GENERATED_BODY()

	/** Returns true if the actor passes the filter and will be targeted */
	virtual bool FilterPassesForActor(const AActor* ActorToBeFiltered) const override;
};
```

但是，这不能直接用于 `Wait Target Data` 节点，因为它需要 `FGameplayTargetDataFilterHandle`。必须创建一个新的自定义 `Make Filter Handle` 来接受子类：
```c++
FGameplayTargetDataFilterHandle UGDTargetDataFilterBlueprintLibrary::MakeGDNameFilterHandle(FGDNameTargetDataFilter Filter, AActor* FilterActor)
{
	FGameplayTargetDataFilter* NewFilter = new FGDNameTargetDataFilter(Filter);
	NewFilter->InitializeFilterContext(FilterActor);

	FGameplayTargetDataFilterHandle FilterHandle;
	FilterHandle.Filter = TSharedPtr<FGameplayTargetDataFilter>(NewFilter);
	return FilterHandle;
}
```

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-targeting-reticles"></a>
#### 4.11.4 Gameplay Ability World Reticles
[`AGameplayAbilityWorldReticles`](https://docs.unrealengine.com/en-US/API/Plugins/GameplayAbilities/Abilities/AGameplayAbilityWorldReticle/index.html) (`Reticles`) 在使用非 `Instant` 确认的 [`TargetActors`](#concepts-targeting-actors) 进行定位时，可视化您的目标对象**是谁**。 `TargetActors` 负责所有 `Reticles` 的生成和销毁生命周期。 `Reticles` 是 `AActors`，因此它们可以使用任何类型的视觉组件进行表示。 [GASShooter](https://github.com/tranek/GASShooter) 中常见的实现是使用 `WidgetComponent` 在屏幕空间中显示 UMG Widget（始终面向玩家的相机）。 `Reticles` 不知道它们位于哪个 `AActor` 上，但您可以在自定义 `TargetActor` 上对该功能进行子类化。 `TargetActors` 通常会将 `Reticle` 的位置更新为每个 `Tick()` 上的目标位置。

GASShooter 使用 `Reticles` 显示火箭发射器辅助能力的自导火箭的锁定目标。敌人身上的红色指示器是`Reticle`。类似的白色图像是火箭发射器的十字准线。
![Reticles in GASShooter](https://github.com/tranek/GASDocumentation/raw/master/Images/gameplayabilityworldreticle.png)

`Reticles` 为设计师提供了一些 `BlueprintImplementableEvents`（它们旨在在蓝图中开发）：

```c++
/** Called whenever bIsTargetValid changes value. */
UFUNCTION(BlueprintImplementableEvent, Category = Reticle)
void OnValidTargetChanged(bool bNewValue);

/** Called whenever bIsTargetAnActor changes value. */
UFUNCTION(BlueprintImplementableEvent, Category = Reticle)
void OnTargetingAnActor(bool bNewValue);

UFUNCTION(BlueprintImplementableEvent, Category = Reticle)
void OnParametersInitialized();

UFUNCTION(BlueprintImplementableEvent, Category = Reticle)
void SetReticleMaterialParamFloat(FName ParamName, float value);

UFUNCTION(BlueprintImplementableEvent, Category = Reticle)
void SetReticleMaterialParamVector(FName ParamName, FVector value);
```

`Reticles` 可以选择使用 `TargetActor` 提供的 [`FWorldReticleParameters`](https://docs.unrealengine.com/en-US/API/Plugins/GameplayAbilities/Abilities/FWorldReticleParameters/index.html) 进行配置。默认结构体只提供一个变量`FVector AOEScale`。虽然您可以在技术上对此结构进行子类化，但 `TargetActor` 将仅接受基本结构。不允许使用默认的 `TargetActors` 对其进行子类化似乎有点短视。但是，如果您制作自己的自定义 `TargetActor`，则可以提供自己的自定义标线参数结构，并在生成它们时手动将其传递给 `AGameplayAbilityWorldReticles` 的子类。

默认情况下不会复制 `Reticles`，但如果您的游戏可以向其他玩家显示本地玩家的目标玩家，则可以进行复制。

`Reticles` 只会在当前有效目标上显示，默认为 `TargetActors`。例如，如果您使用 `AGameplayAbilityTargetActor_SingleLineTrace` 追踪目标，则仅当敌人直接位于追踪路径中时，`Reticle` 才会出现。如果你看向别处，敌人就不再是有效目标，`Reticle`将会消失。如果您希望 `Reticle` 保留在最后一个有效目标上，您需要自定义 `TargetActor` 以记住最后一个有效目标并将 `Reticle` 保留在其上。我将这些称为持久目标，因为它们将持续存在，直到 `TargetActor` 收到确认或取消、`TargetActor` 在其跟踪/重叠中找到新的有效目标，或者目标不再有效（已销毁）。  GASShooter 使用持久目标作为其火箭发射器的辅助能力的寻的火箭瞄准。

**[⬆ 返回顶部](#table-of-contents)**

<a name="concepts-targeting-containers"></a>
#### 4.11.5 Gameplay Effect Container 的目标选择
[`GameplayEffectContainers`](#concepts-ge-containers) 配有可选的高效生产 [`TargetData`](#concepts-targeting-data) 的方法。当 `EffectContainer` 应用在客户端和服务器上时，此定位会立即发生。它比 [`TargetActors`](#concepts-targeting-actors) 更高效，因为它运行在目标对象的 CDO 上（不会生成和销毁 `Actors`），但它缺乏玩家输入，无需确认即可立即发生，无法取消，并且无法从客户端向服务器发送数据（在两者上都产生数据）。它非常适合即时追踪和碰撞重叠。 Epic 的 [动作角色扮演示例项目](https://www.unrealengine.com/marketplace/en-US/product/action-rpg) 的容器包含两种目标定位示例类型 - 定位能力所有者并从事件中拉出 `TargetData`。它还在蓝图中实现了一个，以在距播放器的一定偏移量（由子蓝图类设置）处进行即时球体跟踪。您可以在 C++ 或蓝图中对 `URPGTargetType` 进行子类化，以创建您自己的定位类型。

**[⬆ 返回顶部](#table-of-contents)**

<a name="cae"></a>
## 5. 常见 Ability 和 Effect 的实现

<a name="cae-stun"></a>
### 5.1 眩晕
Typically with stuns, we want to cancel all of a `Character's` active `GameplayAbilities`, prevent new `GameplayAbility` activations, and prevent movement throughout the duration of the stun.示例项目的 Meteor `GameplayAbility` 对命中的目标施加眩晕效果。

要取消目标的主动`GameplayAbilities`，我们在眩晕[`GameplayTag` is added](#concepts-gt-change)时调用`AbilitySystemComponent->CancelAbilities()`。

为了防止新的 `GameplayAbilities` 在眩晕时激活，`GameplayAbilities` 在其 [`Activation Blocked Tags` `GameplayTagContainer`](#concepts-ga-tags) 中被赋予眩晕 `GameplayTag`。

为了防止被击晕时移动，我们重写了 `CharacterMovementComponent's` `GetMaxSpeed()` 函数，当所有者被击晕 `GameplayTag` 时返回 0。

**[⬆ 返回顶部](#table-of-contents)**

<a name="cae-sprint"></a>
### 5.2 冲刺
示例项目提供了如何冲刺的示例 - 在按住 `Left Shift` 的同时跑得更快。

`CharacterMovementComponent` 通过网络向服务器发送标志来预测性地处理更快的移动。详细信息请参见 `GDCharacterMovementComponent.h/cpp`。

`GA` 处理对 `Left Shift` 输入的响应，告诉 `CMC` 开始和停止冲刺，并在按下 `Left Shift` 时预测性地充电耐力。详细信息请参见 `GA_Sprint_BP`。

**[⬆ 返回顶部](#table-of-contents)**

<a name="cae-ads"></a>
### 5.3 开镜瞄准（ADS）
示例项目以与冲刺完全相同的方式处理此问题，但降低而不是增加移动速度。

有关预测性降低移动速度的详细信息，请参阅 `GDCharacterMovementComponent.h/cpp`。

有关处理输入的详细信息，请参阅 `GA_AimDownSight_BP`。瞄准目标不会消耗体力。

**[⬆ 返回顶部](#table-of-contents)**

<a name="cae-ls"></a>
### 5.4 生命偷取
我在伤害 [`ExecutionCalculation`](#concepts-ge-ec) 内处理吸血。 `GameplayEffect` 上将有一个 `GameplayTag`，就像 `Effect.CanLifesteal` 一样。 `ExecutionCalculation` 检查 `GameplayEffectSpec` 是否具有 `Effect.CanLifesteal` `GameplayTag`。如果 `GameplayTag` 存在，则 `ExecutionCalculation` [creates a dynamic `Instant` `GameplayEffect`](#concepts-ge-dynamic) 的生命值作为修正值，并将其应用回 `Source's` `ASC`。

```c++
if (SpecAssetTags.HasTag(FGameplayTag::RequestGameplayTag(FName("Effect.Damage.CanLifesteal"))))
{
	float Lifesteal = Damage * LifestealPercent;

	UGameplayEffect* GELifesteal = NewObject<UGameplayEffect>(GetTransientPackage(), FName(TEXT("Lifesteal")));
	GELifesteal->DurationPolicy = EGameplayEffectDurationType::Instant;

	int32 Idx = GELifesteal->Modifiers.Num();
	GELifesteal->Modifiers.SetNum(Idx + 1);
	FGameplayModifierInfo& Info = GELifesteal->Modifiers[Idx];
	Info.ModifierMagnitude = FScalableFloat(Lifesteal);
	Info.ModifierOp = EGameplayModOp::Additive;
	Info.Attribute = UPAAttributeSetBase::GetHealthAttribute();

	SourceAbilitySystemComponent->ApplyGameplayEffectToSelf(GELifesteal, 1.0f, SourceAbilitySystemComponent->MakeEffectContext());
}
```

**[⬆ 返回顶部](#table-of-contents)**

<a name="cae-random"></a>
### 5.5 在客户端和服务器生成随机数
有时您需要在 `GameplayAbility` 内生成一个“随机”数字，用于子弹后坐力或散布等。客户端和服务器都希望生成相同的随机数。为此，我们必须将 `random seed` 设置为与 `GameplayAbility` 激活时相同。每次激活 `GameplayAbility` 时，您都需要设置 `random seed`，以防客户端错误预测激活并且其随机数序列与服务器的不同步。

| 播种方法                                                          | 描述                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    |
| ---------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 使用激活预测密钥                                            | `GameplayAbility`激活预测密钥是一个int16，保证在`Activation()`中的客户端和服务器中同步且可用。您可以在客户端和服务器上将其设置为 `random seed`。此方法的缺点是每次游戏开始时预测密钥始终从零开始，并在生成密钥之间不断增加要使用的值。这意味着每场比赛都将具有完全相同的随机数序列。这可能足够随机，也可能不够随机以满足您的需求。 |
| 当您激活 `GameplayAbility` 时，通过事件负载发送种子 | 按事件激活 `GameplayAbility`，并通过复制的事件负载将随机生成的种子从客户端发送到服务器。这允许更多的随机性，但客户可以轻松地破解他们的游戏，每次只发送相同的种子值。此外，通过事件激活 `GameplayAbilities` 将阻止它们从输入绑定激活。                                                                                                                                                                     |

如果您的随机偏差很小，大多数玩家不会注意到每个游戏的序列都是相同的，并且使用激活预测密钥作为 `random seed` 应该适合您。如果您正在做一些需要防黑客的更复杂的事情，也许使用 `Server Initiated` `GameplayAbility` 会更好，因为服务器可以创建预测密钥或生成 `random seed` 以通过事件负载发送。

**[⬆ 返回顶部](#table-of-contents)**

<a name="cae-crit"></a>
### 5.6 暴击
我在伤害 [`ExecutionCalculation`](#concepts-ge-ec) 内处理重击。 `GameplayEffect` 上将有一个 `GameplayTag`，就像 `Effect.CanCrit` 一样。 `ExecutionCalculation` 检查 `GameplayEffectSpec` 是否具有 `Effect.CanCrit` `GameplayTag`。如果 `GameplayTag` 存在，`ExecutionCalculation` 会生成一个与暴击几率（从 `Source` 捕获的 `Attribute`）相对应的随机数，如果成功，则添加暴击伤害（也是从 `Source` 捕获的 `Attribute`）。由于我不预测损坏，因此我不必担心同步客户端和服务器上的随机数生成器，因为 `ExecutionCalculation` 只会在服务器上运行。如果您尝试使用 `MMC` 进行损害计算来进行预测，则必须从 `GameplayEffectSpec->GameplayEffectContext->GameplayAbilityInstance` 获取对 `random seed` 的引用。

看看 [GASShooter](https://github.com/tranek/GASShooter) 如何爆头。这是相同的概念，只不过它不依赖于随机数，而是检查 `FHitResult` 骨骼名称。

**[⬆ 返回顶部](#table-of-contents)**

<a name="cae-nonstackingge"></a>
### 5.7 不叠加 Gameplay Effects，仅使数值最大的效果作用于目标
Slow effects in Paragon did not stack.每个慢速实例都正常应用并跟踪其生命周期，但只有最大幅度的慢速效果实际上影响了 `Character`。 GAS 通过 `AggregatorEvaluateMetaData` 开箱即用地提供了这种方案。 See [`AggregatorEvaluateMetaData()`](#concepts-as-onattributeaggregatorcreated) for details and implementation.

**[⬆ 返回顶部](#table-of-contents)**

<a name="cae-paused"></a>
### 5.8 在游戏暂停时生成 Target Data
如果您需要在等待从播放器的 `WaitTargetData` `AbilityTask` 生成 [`TargetData`](#concepts-targeting-data) 时暂停游戏，我建议您不要暂停使用 `slomo 0`。

**[⬆ 返回顶部](#table-of-contents)**

<a name="cae-onebuttoninteractionsystem"></a>
### 5.9 单键交互系统
[GASShooter](https://github.com/tranek/GASShooter)实现了一个一键交互系统，玩家可以按下或按住“E”来与可交互对象进行交互，例如复活玩家、打开武器箱以及打开或关闭滑动门。

**[⬆ 返回顶部](#table-of-contents)**

<a name="debugging"></a>
## 6. 调试 GAS
通常在调试 GAS 相关问题时，您想了解以下信息：
> * “我的属性值是多少？”
> * “我有什么GameplayTag？”
> * “我现在有什么GameplayEffect？”
> * “我授予了哪些能力，哪些能力正在运行，哪些能力被阻止激活？”。

GAS 提供了两种在运行时回答这些问题的技术 - [`showdebug abilitysystem`](#debugging-sd) 和 [`GameplayDebugger`](#debugging-gd) 中的钩子。

**提示：** 虚幻引擎喜欢优化 C++ 代码，这使得某些功能很难调试。当深入跟踪代码时，您很少会遇到这种情况。如果将 Visual Studio 解决方案配置设置为 `DebugGame Editor` 仍会阻止跟踪代码或检查变量，则可以通过使用 `UE_DISABLE_OPTIMIZATION` 和 `UE_ENABLE_OPTIMIZATION` 宏或 CoreMiscDefines.h 中定义的船舶变体包装优化函数来禁用所有优化。除非您从源代码重建插件，否则这不能在插件代码上使用。这可能在内联函数上起作用，也可能不起作用，具体取决于它们的作用和位置。完成调试后请务必删除宏！

```c++
UE_DISABLE_OPTIMIZATION
void MyClass::MyFunction(int32 MyIntParameter)
{
	// My code
}
UE_ENABLE_OPTIMIZATION
```

**[⬆ 返回顶部](#table-of-contents)**

<a name="debugging-sd"></a>
### 6.1 showdebug abilitysystem
在游戏控制台中输入 `showdebug abilitysystem`。此功能分为三个“页面”。所有三个页面都会显示您当前拥有的 `GameplayTags`。在控制台中键入 `AbilitySystem.Debug.NextCategory` 以在页面之间循环。

第一页显示您所有 `Attributes` 的 `CurrentValue`：
![First Page of showdebug abilitysystem](https://github.com/tranek/GASDocumentation/raw/master/Images/showdebugpage1.png)

第二页显示了你身上所有的`Duration`和`Infinite` `GameplayEffects`，它们的堆栈数量，它们给出的`GameplayTags`，以及它们给出的`Modifiers`。
![Second Page of showdebug abilitysystem](https://github.com/tranek/GASDocumentation/raw/master/Images/showdebugpage2.png)

第三页显示了所有已授予您的`GameplayAbilities`，当前是否正在运行，是否被阻止激活，以及当前正在运行的`AbilityTasks`的状态。
![Third Page of showdebug abilitysystem](https://github.com/tranek/GASDocumentation/raw/master/Images/showdebugpage3.png)

要在目标（由 Actor 周围的绿色矩形棱柱表示）之间循环，请使用 `PageUp` 键或 `NextDebugTarget` 控制台命令转到下一个目标，使用 `PageDown` 键或 `PreviousDebugTarget` 控制台命令转到上一个目标。

**注意：** 为了使系统信息能够根据当前选择的调试 Actor 进行更新，您需要在 `AbilitySystemGlobals` 中设置 `bUseDebugTargetFromHud=true`，如下所示在 `DefaultGame.ini` 中：
```
[/Script/GameplayAbilities.AbilitySystemGlobals]
bUseDebugTargetFromHud=true
```

**注意：** 要使 `showdebug abilitysystem` 正常工作，必须在游戏模式中选择实际的 HUD 类。否则找不到该命令并返回“Unknown Command”。

**[⬆ 返回顶部](#table-of-contents)**

<a name="debugging-gd"></a>
### 6.2 Gameplay Debugger
GAS 为Gameplay Debugger添加了功能。使用撇号 (') 键访问Gameplay Debugger。按数字键盘上的 3 启用“能力”类别。根据您拥有的插件，类别可能会有所不同。如果您的键盘没有像笔记本电脑那样的小键盘，那么您可以在项目设置中更改键绑定。

当您想要在 **其他** `Characters` 上查看 `GameplayTags`、`GameplayEffects` 和 `GameplayAbilities` 时，请使用Gameplay Debugger。不幸的是，它没有显示目标 `Attributes` 的 `CurrentValue`。它将瞄准屏幕中央的任何 `Character`。您可以通过在编辑器的 World Outliner 中选择目标或查看不同的 `Character` 并再次按撇号 (') 来更改目标。目前检查的`Character`上面有最大的红圈。

![Gameplay Debugger](https://github.com/tranek/GASDocumentation/raw/master/Images/gameplaydebugger.png)

**[⬆ 返回顶部](#table-of-contents)**

<a name="debugging-log"></a>
### 6.3 GAS 日志
GAS 源代码包含大量以不同详细级别生成的日志记录语句。您很可能会将这些视为 `ABILITY_LOG()` 语句。默认详细级别为 `Display`。默认情况下，任何更高的值都不会显示在控制台中。

要更改日志类别的详细级别，请在控制台中输入：

```
log [category] [verbosity]
```

例如，要打开 `ABILITY_LOG()` 语句，您可以在控制台中输入：
```
log LogAbilitySystem VeryVerbose
```

要将其重置回默认值，请键入：
```
log LogAbilitySystem Display
```

要显示所有日志类别，请键入：
```
log list
```

值得注意的 GAS 相关日志类别：

| 日志类别          | 默认详细级别 |
| ------------------------- | ----------------------- |
| 日志能力系统          | 展示                 |
| LogAbility系统组件 | 日志                     |
| 日志GameplayCue详情     | 日志                     |
| 日志GameplayCue翻译器  | 展示                 |
| 日志GameplayEffect详情  | 日志                     |
| 日志GameplayEffect        | 展示                 |
| 日志GameplayTag           | 日志                     |
| 记录游戏任务          | 日志                     |
| 视频记录能力系统         | 展示                 |

有关详细信息，请参阅 [维基日志](https://unrealcommunity.wiki/logging-lgpidy6i)。

**[⬆ 返回顶部](#table-of-contents)**

<a name="optimizations"></a>
## 7. 优化

<a name="optimizations-abilitybatching"></a>
### 7.1 Ability 批处理
激活[`GameplayAbilities`](#concepts-ga)，可选发送`TargetData`到服务器，一帧结束可以是[批量将两到三个 RPC 压缩为一个 RPC](#concepts-ga-batching)。这些类型的能力通常用于命中扫描枪。

<a name="optimizations-gameplaycuebatching"></a>
### 7.2 Gameplay Cue 批处理
如果您同时发送多个 [`GameplayCues`](#concepts-gc)，请考虑 [将它们批处理到一个 RPC 中](#concepts-gc-batching)。目标是减少 RPC 的数量（`GameplayCues` 是不可靠的 NetMulticast）并发送尽可能少的数据。

<a name="optimizations-ascreplicationmode"></a>
### 7.3 AbilitySystemComponent 的 Replication Mode
默认情况下，[`ASC`](#concepts-asc) 位于 [`Full Replication Mode`](#concepts-asc-rm)。这会将所有 [`GameplayEffects`](#concepts-ge) 复制到每个客户端（这对于单人游戏来说很好）。在多人游戏中，将玩家拥有的`ASCs`设置为`Mixed Replication Mode`，将AI控制的角色设置为`Minimal Replication Mode`。这将复制应用于玩家角色的 `GEs`，仅复制到该角色的所有者，而应用于 AI 控制角色的 `GEs` 永远不会将 `GEs` 复制到客户端。无论 `Replication Mode` 为何，[`GameplayTags`](#concepts-gt) 仍将进行复制，并且 [`GameplayCues`](#concepts-gc) 仍将是对所有客户端不可靠的 NetMulticast。当所有客户端不需要查看 `GEs` 的网络数据时，这将减少复制它们。

<a name="optimizations-attributeproxyreplication"></a>
### 7.4 Attribute 代理复制
在 Fortnite Battle Royale（FNBR）这样的大型游戏中，大量 [`ASCs`](#concepts-asc) 位于始终相关（always relevant）的 `PlayerStates` 上，并需要复制许多 [`Attributes`](#concepts-a)。为了优化这一瓶颈，Fortnite 在 `PlayerState::ReplicateSubobjects()` 中，对**玩家控制角色的 simulated proxies** 完全禁用了 `ASC` 及其 [`AttributeSets`](#concepts-as) 的复制。Autonomous proxies 和 AI 控制的 `Pawns` 仍然按各自的 [`Replication Mode`](#concepts-asc-rm) 正常复制。FNBR 不再从始终相关的 `PlayerStates` 上的 `ASC` 复制 `Attributes`，而是在玩家的 `Pawn` 上使用一个可复制的代理结构体。服务器 `ASC` 上的 `Attributes` 改变时，也会同步更新代理结构体。客户端收到结构体中复制的 `Attributes` 后，再将变化写回本地 `ASC`。这样，`Attribute` 的复制就可以利用 `Pawn` 的相关性和 `NetUpdateFrequency`。代理结构体还通过 bitmask 复制少量白名单内的 `GameplayTags`。这项优化既减少了网络数据量，又能利用 Pawn 的相关性。AI 控制的 `Pawns` 已经将 `ASC` 放在 `Pawn` 上，并使用其相关性，因此不需要这项优化。

> 我不确定自那时以来所做的其他服务器端优化（复制图等）是否仍然有必要，而且它不是最可维护的模式。

*Epic 的 Dave Ratti 对 [社区问题 #3](https://epicgames.ent.box.com/s/m1egifkxv3he3u3xezb9hzbgroxyhx89)* 的回答

<a name="optimizations-asclazyloading"></a>
### 7.5 ASC 延迟加载
Fortnite Battle Royale (FNBR) 世界上有很多可损坏的 `AActors`（树木、建筑物等），每个都有一个 [`ASC`](#concepts-asc)。这会增加内存成本。 FNBR 通过仅在需要时（当它们第一次受到玩家伤害时）延迟加载 `ASCs` 来优化这一点。这减少了总体内存使用量，因为某些 `AActors` 在比赛中可能永远不会损坏。

**[⬆ 返回顶部](#table-of-contents)**

<a name="qol"></a>
## 8. 改善开发体验的建议

<a name="qol-gameplayeffectcontainers"></a>
### 8.1 Gameplay Effect Containers
[GameplayEffectContainers](#concepts-ge-containers) 将 [`GameplayEffectSpecs`](#concepts-ge-spec)、[`TargetData`](#concepts-targeting-data)、[简单的目标定位](#concepts-targeting-containers) 和相关功能组合到易于使用的结构中。这些非常适合将 `GameplayEffectSpecs` 转移到由技能生成的射弹，然后将它们应用于稍后的碰撞。

<a name="qol-asynctasksascdelegates"></a>
### 8.2 通过蓝图 AsyncTask 绑定 ASC 的 Delegates
为了增加设计人员友好的迭代时间，特别是在设计 UI 的 UMG Widget 时，请创建 Blueprint AsyncTasks（C++ 语言）以直接从 UMG Blueprint 图表绑定到 `ASC` 上的常见变更委托。唯一需要注意的是，它们必须手动销毁（就像销毁小部件时一样），否则它们将永远存在于内存中。示例项目包括三个蓝图 AsyncTask。

监听 `Attribute` 的变化：

![Listen for Attributes Changes BP Node](https://github.com/tranek/GASDocumentation/raw/master/Images/attributeschange.png)

监听冷却时间的变化：

![Listen for Cooldown Change BP Node](https://github.com/tranek/GASDocumentation/raw/master/Images/cooldownchange.png)

监听 `GE` 堆栈更改：

![Listen for GameplayEffect Stack Change BP Node](https://github.com/tranek/GASDocumentation/raw/master/Images/gestackchange.png)

**[⬆ 返回顶部](#table-of-contents)**

<a name="troubleshooting"></a>
## 9. 故障排查

<a name="troubleshooting-notlocal"></a>
### 9.1 `LogAbilitySystem: Warning: Can't activate LocalOnly or LocalPredicted ability %s when not local!`
您需要 [initialize the `ASC` on the client](#concepts-asc-setup)。

**[⬆ 返回顶部](#table-of-contents)**

<a name="troubleshooting-scriptstructcache"></a>
### 9.2 `ScriptStructCache` 错误
您需要拨打[`UAbilitySystemGlobals::InitGlobalData()`](#concepts-asg-initglobaldata)。

**[⬆ 返回顶部](#table-of-contents)**

<a name="troubleshooting-replicatinganimmontages"></a>
### 9.3 Animation Montage 未复制到客户端
确保您在 [GameplayAbilities](#concepts-ga) 中使用 `PlayMontageAndWait` 蓝图节点而不是 `PlayMontage`。该 [AbilityTask](#concepts-at) 自动通过 `ASC` 复制剪辑，而 `PlayMontage` 节点则不会。

**[⬆ 返回顶部](#table-of-contents)**

<a name="troubleshooting-duplicatingblueprintactors"></a>
### 9.4 复制蓝图 Actor 导致 AttributeSet 变为 nullptr
对于从现有 Blueprint Actor 类复制的 Blueprint Actor 类，有一个 [虚幻引擎中的错误](https://issues.unrealengine.com/issue/UE-81109) 会将类上的 `AttributeSet` 指针设置为 nullptr。有一些解决方法。我已经成功地不在我的类上创建定制的 `AttributeSet` 指针（.h 中没有指针，不在构造函数中调用 `CreateDefaultSubobject`），而是直接将 `AttributeSets` 添加到 `PostInitializeComponents()` 中的 `ASC` （示例项目中未显示）。复制的 `AttributeSets` 仍将存在于 `ASC's` `SpawnedAttributes` 阵列中。它看起来像这样：

```c++
void AGDPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->AddSet<UGDAttributeSetBase>();
		// ... any other AttributeSets that you may have
	}
}
```

在这种情况下，您将使用 `ASC`（而不是 [calling functions on the `AttributeSet` made from the macros](#concepts-as-attributes)）上的函数读取和设置 `AttributeSet` 中的值。

```c++
/** Returns current (final) value of an attribute */
float GetNumericAttribute(const FGameplayAttribute &Attribute) const;

/** Sets the base value of an attribute. Existing active modifiers are NOT cleared and will act upon the new base value. */
void SetNumericAttributeBase(const FGameplayAttribute &Attribute, float NewBaseValue);
```

所以 `GetHealth()` 看起来像：

```c++
float AGDPlayerState::GetHealth() const
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->GetNumericAttribute(UGDAttributeSetBase::GetHealthAttribute());
	}

	return 0.0f;
}
```

设置（初始化）健康状况 `Attribute` 看起来像：

```c++
const float NewHealth = 100.0f;
if (AbilitySystemComponent)
{
	AbilitySystemComponent->SetNumericAttributeBase(UGDAttributeSetBase::GetHealthAttribute(), NewHealth);
}
```

提醒一下，`ASC` 只期望每个 `AttributeSet` 类最多有一个 `AttributeSet` 对象。

**[⬆ 返回顶部](#table-of-contents)**

<a name="troubleshooting-unresolvedexternalsymbolmarkpropertydirty"></a>
### 9.5 无法解析外部符号 UEPushModelPrivate::MarkPropertyDirty(int,int)

如果您收到类似以下的编译器错误：

```
error LNK2019: unresolved external symbol "__declspec(dllimport) void __cdecl UEPushModelPrivate::MarkPropertyDirty(int,int)" (__imp_?MarkPropertyDirty@UEPushModelPrivate@@YAXHH@Z) referenced in function "public: void __cdecl FFastArraySerializer::IncrementArrayReplicationKey(void)" (?IncrementArrayReplicationKey@FFastArraySerializer@@QEAAXXZ)
```

这是尝试在 `FFastArraySerializer` 上调用 `MarkItemDirty()` 时产生的。我在更新 `ActiveGameplayEffect` 时遇到过这种情况，例如更新冷却时间时。

```c++
ActiveGameplayEffects.MarkItemDirty(*AGE);
```

发生的情况是 `WITH_PUSH_MODEL` 在多个地方被定义。 `PushModelMacros.h` 将其定义为 0，而在多个地方将其定义为 1。 `PushModel.h` 将其视为 1，但 `PushModel.cpp` 将其视为 0。

解决方案是将`NetCore`添加到您项目的`PublicDependencyModuleNames`中的`Build.cs`中。

**[⬆ 返回顶部](#table-of-contents)**

<a name="troubleshooting-enumnamesarenowpathnames"></a>
### 9.6 Enum 名称现在以路径名称表示

如果您收到类似以下的编译器警告：

```
warning C4996: 'FGameplayAbilityInputBinds::FGameplayAbilityInputBinds': Enum names are now represented by path names. Please use a version of FGameplayAbilityInputBinds constructor that accepts FTopLevelAssetPath. Please update your code to the new API before upgrading to the next release, otherwise your project will no longer compile.
```

UE 5.1 已弃用在 `BindAbilityActivationToInputComponent()` 的构造函数中使用 `FString`。相反，我们必须传入 `FTopLevelAssetPath`。

旧的、已弃用的方式：
```c++
AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
	FString("CancelTarget"), FString("EGDAbilityInputID"), static_cast<int32>(EGDAbilityInputID::Confirm), static_cast<int32>(EGDAbilityInputID::Cancel)));
```

新方式：
```c++
FTopLevelAssetPath AbilityEnumAssetPath = FTopLevelAssetPath(FName("/Script/GASDocumentation"), FName("EGDAbilityInputID"));
AbilitySystemComponent->BindAbilityActivationToInputComponent(InputComponent, FGameplayAbilityInputBinds(FString("ConfirmTarget"),
	FString("CancelTarget"), AbilityEnumAssetPath, static_cast<int32>(EGDAbilityInputID::Confirm), static_cast<int32>(EGDAbilityInputID::Cancel)));
```

有关详细信息，请参阅 `Engine\Source\Runtime\CoreUObject\Public\UObject\TopLevelAssetPath.h`。

**[⬆ 返回顶部](#table-of-contents)**

<a name="acronyms"></a>
## 10. GAS 常用缩写

| 姓名                                                                                                   | 缩略语            |
|------------------------------------------------------------------------------------------------------- | ------------------- |
| AbilitySystemComponent                                                                                 | ASC                 |
| AbilityTask                                                                                            | 在                  |
| [Epic 的动作角色扮演示例项目](https://www.unrealengine.com/marketplace/en-US/product/action-rpg) | ARPG, ARPG 示例   |
| 角色动作组件                                                                             | 羧甲基纤维素钠                 |
| GameplayAbility                                                                                        | 遗传算法                  |
| GameplayAbility系统                                                                                  | 气体                 |
| GameplayCue                                                                                            | 气相色谱                  |
| GameplayEffect                                                                                         | 通用电气                  |
| GameplayEffect执行计算                                                                     | ExecCalc，执行 |
| GameplayTag                                                                                            | 标签，GT             |
| 修改量计算                                                                           | ModMagCalc、MMC     |

**[⬆ 返回顶部](#table-of-contents)**

<a name="resources"></a>
## 11. 其他资源
* [官方文档](https://docs.unrealengine.com/en-US/Gameplay/GameplayAbilitySystem/index.html)
* 源代码！
   * 特别是`GameplayPrediction.h`
* [Epic 的 Lyra 示例项目](https://unrealengine.com/marketplace/en-US/learn/lyra)
* [Epic 的动作角色扮演示例项目](https://www.unrealengine.com/marketplace/en-US/product/action-rpg)
* [虚幻懒鬼不和谐](https://unrealslackers.org/) 有一个专用于 GAS `#gameplay-ability-system` 的文本通道
   * 检查固定消息
* [Dan 'Pan' 的 GitHub 资源库](https://github.com/Pantong51/GASContent)
* [SabreDartStudios 的 YouTube 视频](https://www.youtube.com/channel/UCCFUhQ6xQyjXDZ_d6X_H_-A)

<a name="resources-daveratti"></a>
### 11.1 与 Epic Games 的 Dave Ratti 问答

<a name="resources-daveratti-community1"></a>
#### 11.1.1 社区问题（一）
[Dave Ratti 回答 Unreal Slackers Discord Server 社区有关 GAS 的问题](https://epicgames.ent.box.com/s/m1egifkxv3he3u3xezb9hzbgroxyhx89):

1. 我们如何在 `GameplayAbilities` 之外按需创建范围预测窗口？例如，发射后不管的射弹如何在击中敌人时局部预测伤害 `GameplayEffect`？

> PredictionKey 系统并不是真的要这样做。从根本上讲，该系统的工作原理是客户端启动预测操作，用密钥告诉服务器有关该操作的信息，然后客户端和服务器都运行相同的操作，并将预测副作用与给定的预测密钥相关联。例如，“我正在预测性地激活一项能力”或“我已经生成了目标数据，并将在 WaitTargetData 任务之后预测性地运行能力图的一部分”。
>
> 通过这种模式，PredictionKey 从服务器“弹回”并通过 UAbilitySystemComponent::ReplicatedPredictionKeyMap（复制属性）返回客户端。一旦密钥从服务器复制回来，客户端就能够撤消所有本地预测副作用（GameplayCues、GameplayEffects）：复制的版本*将在那里*，如果不存在，则这是一个错误预测。准确地知道何时消除预测副作用至关重要：如果太早，您会看到差距，如果太晚，您将看到“双重”。 （请注意，这是指状态预测，例如基于GameplayEffect的持续时间的循环 GameplayCue。“突发”GameplayCues 和即时 Gameplay 效果永远不会“撤消”或回滚。如果存在与它们关联的预测键，它们只是在客户端上跳过）。
>
> 为了进一步切中要害：至关重要的是，预测操作是服务器不会自行执行的操作，而是仅在客户端告诉它们时才执行。因此，通用的“按需创建密钥并告诉服务器以便我可以运行某些内容”是行不通的，除非该“某些内容”是服务器仅在客户端告知后才会执行的操作。
>
> 回到最初的问题：像着火后忘记弹丸之类的东西。 Paragon 和 Fornite 都有使用 GameplayCues 的投射物 Actor 类。然而，我们不使用预测密钥系统来执行这些操作。相反，我们有一个关于非复制GameplayCue的概念。仅在本地触发并被服务器完全跳过的GameplayCue。本质上，所有这些都是对 UGameplayCueManager::HandleGameplayCue 的直接调用。它们不通过 UAbilitySystemComponent 进行路由，因此不会进行预测密钥检查/提前返回。
>
> 非复制 GameplayCues 的缺点是，它们没有被复制。因此，由射弹类/蓝图来确保调用这些函数的代码路径在每个人上运行。我们有启动（在 BeginPlay 中调用）、爆炸、撞墙/角色等提示。
>
> 这些类型的事件已经在客户端生成，因此调用非复制的GameplayCue没什么大不了的。复杂的蓝图可能很棘手，并且需要作者确保他们了解什么在哪里运行。

2. 当使用 `WaitNetSync` `AbilityTask` 和 `OnlyServerWait` 在本地预测的 `GameplayAbility` 中创建范围预测窗口时，玩家是否可能通过延迟到服务器的数据包来控制 `GameplayAbility` 计时来作弊，因为服务器正在等待带有预测密钥的 RPC？ Paragon 或 Fortnite 中是否曾出现过这个问题？如果是，Epic 采取了什么措施来解决这个问题？

> 是的，这是一个合理的担忧。在等待客户端“信号”的服务器上运行的任何能力蓝图都可能容易受到滞后开关类型的攻击。
>
> Paragon 有一个类似于 UAbilityTask_WaitTargetData 的自定义定位任务。在此任务中，我们有超时或“最大延迟”，我们将在客户端上等待瞬时定位模式。如果目标模式正在等待用户确认（按下按钮），那么它将被忽略，因为用户可以慢慢来。但对于立即确认目标的能力，我们只会等待一定的时间，然后A）生成目标数据服务器端或B）取消该能力。
>
> 我们从未有过这样的 WaitNetSync 机制，我们很少使用它。
>
> 但我不相信《堡垒之夜》会使用这样的东西。 《堡垒之夜》中的武器能力经过特殊处理，批量发送到单个《堡垒之夜》特定的 RPC：一个 RPC 可以激活该能力、提供目标数据并结束该能力。因此，在大逃杀中，武器能力本质上不会受到此影响。
>
> 我的看法是，这可能是可以在整个系统范围内解决的问题，但我认为我们不会很快做出改变。现场修复 WaitNetSync 以包括您提到的情况的最大延迟可能是一项合理的任务，但同样 - 我们不太可能在不久的将来这样做。


3. Paragon 和 Fortnite 使用了哪款 `EGameplayEffectReplicationMode`？Epic 对于何时使用它们的建议是什么？

> 这两款游戏本质上都对玩家控制的角色使用混合模式，对人工智能控制的角色使用最小模式（人工智能小兵、丛林小兵、人工智能尸壳等）。这是我建议大多数人在多人游戏中使用该系统的方式。在项目中越早设置这些越好。
>
> 《堡垒之夜》在优化方面更进一步。实际上，它根本不会为模拟代理复制 UAbilitySystemComponent。组件和属性子对象在拥有的 Fortnite 玩家状态类上的 ::ReplicateSubobjects() 内跳过。我们确实将最基本的复制数据从AbilitySystemComponent推送到 pawn 本身的结构（基本上是我们在位掩码中复制的属性值的子集和标签的白名单子集）。我们称之为“代理”。在接收端，我们获取在 pawn 上复制的代理数据，并将其推回到玩家状态的AbilitySystemComponent中。因此，FNBR 中的每个玩家确实都有一个 ASC，它只是不直接复制：相反，它通过 pawn 上的最小代理结构复制数据，然后路由回接收端的 ASC。这是一个优点，因为它 A) 更小的数据集 B) 利用了Pawn相关性。
>
> 我不确定自那时以来所做的其他服务器端优化（复制图等）是否仍然有必要，而且它不是最可维护的模式。


4. 由于我们无法根据 `GameplayPrediction.h` 预测 `GameplayEffects` 的删除，是否有任何策略可以减轻延迟对删除 `GameplayEffects` 的影响？例如，当移除移动速度较慢时，我们目前必须等待服务器复制 `GameplayEffect` 移除操作，从而捕捉到玩家的角色位置。

> 这是一个很难的问题，我没有一个好的答案。我们通常通过公差和平滑来回避这些问题。我完全同意能力系统和与角色运动系统的精确同步不是一个好的地方，也是我们确实想要修复的地方。
>
> 我有一个允许预测性删除 GE 的架子，但在必须继续之前永远无法解决所有边缘情况。但这并不能解决所有问题，因为角色移动仍然有一个内部保存的移动缓冲区，它对能力系统和可能的移动速度Modifier等一无所知。即使在无法预测 GE 移除的情况下，仍然有可能进入校正反馈循环。
>
> 如果你认为你遇到了真正绝望的情况，你可以预测性地添加一个会抑制你移动速度的 GE。我自己从来没有这样做过，但之前已经对此进行了理论分析。它可能能够帮助解决某一类问题。


5. 我们知道 `AbilitySystemComponent` 位于 Paragon 和 Fortnite 中的 `PlayerState` 上，以及 Action RPG Sample 中的 `Character` 上。 Epic 关于 SkillSystemComponent 应驻留位置的内部规则、指南或建议是什么，以及其 `Owner` 应该是什么？

> 一般来说，我会说任何不需要重生的东西都应该让所有者和阿凡达Actor是同一件事。任何 AI 敌人、建筑物、世界道具等。
>
> 任何重生的东西都应该有不同的所有者和化身，这样AbilitySystemComponent就不需要在重生后保存/重新创建/恢复。 PlayerState 是逻辑选择，它被复制到所有客户端（而 PlayerController 则不然）。缺点是 PlayerStates 总是相关的，因此您可能会在 100 名玩家的游戏中遇到问题（请参阅问题 #3 中 FN 所做的说明）。


6. 是否可以拥有多个具有相同所有者但不同Avatar的 `AbilitySystemComponents`（例如，在Pawn和武器/物品/射弹上将 `Owner` 设置为 `PlayerState`）？

> 我看到的第一个问题是在拥有的 Actor 上实现 IGameplayTagAssetInterface 和 IAbilitySystemInterface。前者可能是可能的：仅聚合来自所有 ASC 的标签（但请注意 - HasAllMatchingGameplayTags 可能只能通过跨 ASC 聚合来满足。仅将调用转发到每个 ASC 并将结果或在一起是不够的）。但后者更加棘手：哪一个 ASC 才是权威的？如果有人想要申请 GE - 哪一个应该获得它？也许你可以解决这些问题，但问题的这一面将是最困难的：所有者将在他们下面放置多个 ASC。
>
> 不过，Pawn和武器上的单独 ASC 本身就有意义。例如，区分描述武器的标签和描述拥有Pawn的标签。也许授予武器的标签也“适用于”所有者而不是其他东西确实有意义（例如，属性和 GE 是独立的，但所有者将像我上面描述的那样聚合拥有的标签）。我确信这会成功。但同一所有者拥有多个 ASC 可能会很冒险。


7. 有没有办法阻止服务器覆盖拥有客户端的本地预测能力的冷却时间？在高延迟的情况下，这将使拥有的客户端在其本地冷却时间到期但在服务器上仍处于冷却状态时“尝试”再次激活该能力。当拥有客户端的激活请求通过网络到达服务器时，服务器可能已停止冷却，或者服务器可能能够将激活请求排队等待其剩余的毫秒。否则，与延迟较低的客户端相比，延迟较高的客户端在重新激活功能之前会有更长的延迟。这在冷却时间非常低的能力（例如冷却时间可能少于一秒的基本攻击）中最为明显。如果没有办法阻止服务器覆盖本地预测能力的冷却时间，那么 Epic 减轻高延迟对重新激活能力的影响的策略是什么？换句话说，Epic是如何设计Paragon的基础攻击和其他能力，让高延迟玩家能够以与低延迟玩家相同的速度进行本地预测的攻击或激活？

> 简而言之，没有办法阻止这种情况发生，Paragon 肯定有这个问题。较高的延迟连接在基本攻击中具有较低的 ROF。
>
> 我尝试通过添加“GE 协调”来解决此问题，其中在计算 GE 持续时间时考虑了延迟。本质上是允许服务器占用总 GE 时间的一部分，以便 GE 客户端的有效时间与任何延迟量都 100% 一致（尽管波动仍然可能导致问题）。然而，我从来没有让它在可以交付的状态下工作，而且项目进展得很快，我们只是从未完全解决它。
>
> Fortnite 自己记录武器射击频率：它不使用 GE 来计算武器的冷却时间。如果这对您的游戏来说是一个关键问题，我会推荐这样做。


8. Epic 针对 GameplayAbilitySystem 插件的路线图是什么？ Epic 计划在 2019 年及以后添加哪些功能？

> 我们认为目前整个系统相当稳定，我们没有任何人致力于主要的新功能。有时会针对《堡垒之夜》或 UDN/拉取请求进行错误修复和小改进，但现在就是这样。
>
> 从长远来看，我认为我们最终会进行“V2”或一些重大改变。我们从编写这个系统中学到了很多东西，并且感觉我们有很多正确的地方，也有很多错误的地方。我希望有机会纠正这些错误并改进上面指出的一些致命缺陷。
>
> 如果 V2 即将到来，提供升级路径将至关重要。我们永远不会制作 V2，而将《堡垒之夜》永远保留在 V1 上：会有一些路径或程序会尽可能自动迁移，尽管几乎肯定仍然需要一些手动重制。
>
> 高优先级修复将是：
> * 与角色移动系统更好的互操作性。统一客户预测。
> * GE 去除预测（问题#4）
> * GE 延迟协调（问题#7）
> * 通用网络优化，例如批处理 RPC 和代理结构。主要是我们为《堡垒之夜》所做的事情，但找到了将其分解为更通用形式的方法，至少这样游戏就可以更轻松地编写自己的游戏特定优化。
>
> 我会考虑进行更一般的重构类型的更改：
> * 我希望从根本上摆脱让 GE 直接引用电子表格值的做法，相反，它们将能够发出参数，并且这些参数可以由绑定到电子表格值的一些更高级别的对象来填充。当前模型的问题在于，GE 由于与曲线表行紧密耦合而变得不可共享。我认为可以编写一个通用的参数化系统，并将其作为 V2 系统的基础。
> * 减少 UGameplayAbility 上的“策略”数量。我会删除 ReplicationPolicy 和 InstancingPolicy。在我看来，复制实际上几乎从未被需要，并且会导致混乱。应通过将 FGameplayAbilitySpec 设为可子类化的 UObject 来替换 InstancingPolicy。这应该是具有事件且可蓝图化的“非实例化能力对象”。 UGameplayAbility 应该是“每次执行实例化”对象。如果您需要实际实例化，它可以是可选的：相反，“非实例化”能力将通过新的 UGameplayAbilitySpec 对象来实现。 
> * 系统应该提供更多“中层”构造，例如“过滤的 GE 应用程序容器”（数据驱动哪些 GE 应用到具有更高级别游戏逻辑的参与者）、“重叠体积支持”（基于碰撞原语重叠事件应用“过滤的 GE 应用程序容器”）等。这些是每个项目最终以自己的方式实现的构建块。让它们正确并非易事，所以我认为我们应该做得更好，提供一些基本的实现。 
> * 一般来说，减少项目启动和运行所需的样板文件。可能是一个单独的模块“Ex库”或任何可以提供诸如被动能力或开箱即用的基本命中扫描武器之类的东西。该模块是可选的，但可以让您快速启动并运行。
> * 我想将 GameplayCues 移至不与能力系统耦合的单独模块。我认为这里可以进行很多改进。


> 这仅是我个人的意见，不代表任何人的承诺。我认为最现实的行动方案将是随着新的引擎技术计划的实施，能力系统将需要更新，这将是做此类事情的时候。这些举措可能与脚本、网络或物理/角色运动有关。不过，这一切都是非常遥远的未来，所以我无法对时间表做出承诺或估计。

**[⬆ 返回顶部](#table-of-contents)**

<a name="resources-daveratti-community2"></a>
#### 11.1.2 社区问题（二）
社区成员 [iniside](https://github.com/iniside) 与 Dave Ratti 的问答：

1. 是否计划支持解耦固定报价？我想
固定游戏线程（如 30/60fps）并让渲染线程
狂奔。我问这是我们未来应该期待的事情还是
不是，对游戏玩法应该如何运作做出一些假设。
我问主要是因为现在有一个固定的物理异步刻度
这就提出了一个问题：系统的其余部分如何在
未来。我并不隐瞒有能力修复滴答游戏
线程而不固定引擎其余部分的滴答率
太棒了。

> 没有计划将渲染帧速率和游戏线程滴答帧速率解耦。我认为由于这些系统的复杂性以及保持与先前发动机版本的向后兼容性的要求，这艘船已经在这种情况下航行了。
>
> 相反，我们的方向是拥有一个异步“物理线程”，它以固定的滴答率运行，独立于游戏线程。需要以固定速率运行的东西可以在这里运行，游戏线程/渲染可以按照它们一贯的方式运行。
>
> 值得澄清的是，网络预测支持所谓的独立计时和固定计时模式。我的长期计划是保持独立滴答在网络预测中的大致样子，它以可变帧速率在游戏线程上运行，并且没有“组/世界”预测，它只是经典的“客户端预测他们自己的Pawn和拥有的Actor”模型。固定滴答将使用异步物理内容，并允许您预测非客户端控制/拥有的参与者，例如物理对象和其他客户端/Pawn/车辆/等。


2. 是否有关于如何整合网络预测的计划？
用能力系统看看？例如，固定框架能力
激活（因此服务器获得能力被激活的帧
激活并执行任务而不是预测键）？

> 是的，计划是重写/删除能力系统的预测密钥并用网络预测结构替换它们。 NetworkPredictionExtras 中的 MockAbility 示例展示了其工作原理，但它们比 GAS 所需的更加“硬编码”。 
>
> 主要思想是我们删除 ASC RPC 中显式的客户端->服务器预测密钥交换。将不再有预测窗口或范围预测键。相反，一切都将围绕 NetworkPrediction 框架进行。重要的是客户端和服务器在事情发生时达成一致。例子是：
>
> * 能力何时被激活/结束/取消
> * 何时应用/删除GameplayEffect
> * 属性值（帧 X 处的属性值是什么）
>
> 我认为这通常可以在能力系统层面上完成。但实际上使 UGameplayAbility 内的用户定义逻辑完全可回滚仍然需要更多工作。我们最终可能会得到一个完全可回滚的 UGameplayAbility 子类，并且可以访问一组更有限的功能，或者只能访问标记为回滚友好的AbilityTask。类似的事情。对于动画事件和Root Motion以及它们的处理方式也有很多影响。
>
> 希望我有一个更明确的答案，但在再次接触 GAS 之前，我们必须打好基础，这一点非常重要。在改变更高层次的系统之前，运动和物理必须是可靠的。


3. 是否有计划将网络预测发展推向
主要分支？不会撒谎，我真的很想检查最新的代码。
不管它的状态如何。

> 我们正在为此努力。系统工作仍然全部在 NetworkPrediction 中完成（请参阅 NetworkPhysics.h），并且底层异步物理内容应该全部可用（RewindData.h 等）。但我们在《堡垒之夜》中也有我们一直关注的用例，但显然不能公开。我们正在解决错误、性能优化等。
>
> 更多背景信息：在开发该系统的早期版本时，我们非常关注事物的“前端”——如何定义和编写状态和模拟。我们在那里学到了很多东西。但随着异步物理的东西已经上线，我们更加专注于让一些真实的东西在这个系统中工作，而代价是抛弃我们早期的一些抽象。这里的目标是回到真正的事情正在发挥作用并重新统一事物的时候。例如，回到“前端”并在我们现在正在研究的核心技术之上制作最终版本。


4. 一段时间以来，主分支上有一个用于发送游戏玩法的插件
消息（看起来像事件/消息总线），但已被删除。任何
计划恢复吗？通过游戏功能/模块化游戏插件，
拥有一个通用的事件总线调度程序将非常有用。

> 我认为您指的是 GameplayMessages 插件。这可能会在某个时候再次出现——API 还没有真正最终确定，而且作者并没有打算将其公开。我同意它对于模块化游戏设计应该很有用。但这不是我的领域，所以我没有更多信息。 


5. 我最近一直在玩异步固定物理和结果
很有希望，不过如果将来有 NP 更新的话
我可能会只是玩玩并等待，因为为了让它发挥作用，我
仍然需要让整个引擎进入固定状态，另一方面
我尝试将物理速度保持在 33 毫秒。这并没有带来好处
体验如果一切都在 30 fps (:.

我注意到有一些关于异步的工作
CharacterMovementComponent，但不确定这是否会使用网络
预测，还是单独的努力？

既然我注意到了，我也继续尝试实现我的自定义
以固定的滴答率进行异步运动，效果还不错，但最重要的是
我还需要添加一个单独的插值更新。设置
是以固定的 33 毫秒在单独的工作线程上运行模拟刻度
更新、计算、保存结果并在游戏中进行插值
线程匹配当前帧速率。不完美，但它得到了这份工作
完毕。

我的问题是，这是否可能更容易设置
未来，因为只有相当多的样板代码需要编写，
（插值部分）并且它不是特别有效
分别对每个移动对象进行插值。

异步的东西真的很有趣，因为它可以让你
真正以固定的更新速率运行游戏模拟（这将使
不需要固定线程）并具有更可预测的结果。这是
打算继续进行的事情，或者更有利于
选择系统？据我记得Actor变换没有更新
异步和蓝图并不完全线程安全。换句话说就是
它计划在更多的框架中得到支持
关卡还是每个游戏必须自己解决的问题？

> 异步角色移动组件
>
> 这基本上是 CMC 移植到物理线程的早期原型/实验。我还不认为它是 CMC 的未来，但它可能会发展成那样。目前还没有网络支持，所以我不会真正关注它。从事这项工作的人主要关心的是测量该系统将增加的输入延迟以及如何减轻这种延迟。
>
> 我仍然需要让整个引擎进入固定滴答状态，另一方面我尝试将物理速度保持在 33 毫秒。如果一切都是 30 fps (:.
>
> 异步的东西真的很有趣，因为它允许你真正以固定的更新速率运行游戏模拟（这将使固定线程变得不需要） 
>
> 是的。这里的目标是，启用异步物理后，您可以以可变滴答速率运行引擎，而物理和“核心”游戏模拟可以以固定速率运行（例如角色移动、车辆、气体等）。
>
> 这些是现在需要设置来启用此功能的 cvar：（我认为您已经弄清楚了）  
> `p.DefaultAsyncDt=0.03333`  
> `p.RewindCaptureNumFrames=64`
>
> Chaos 确实为物理状态提供了插值（例如，被推回到 UPrimitiveComponent 并对游戏代码可见的变换）。现在有一个 cvar，`p.AsyncInterpolationMultiplier`，如果您想查看它，它可以控制它。您应该看到物理体的平滑连续运动，而无需编写任何额外的代码。 
>
> 如果您想插入非物理状态，现在仍然由您决定。该示例就像您想要在异步物理线程上更新（勾选）的冷却，但在游戏线程上看到平滑的连续插值，以便更新每个渲染帧的冷却可视化。我们最终会谈到这一点，但还没有例子。
>
> 只需要编写相当多的样板代码，
>
> 是的，到目前为止，这一直是系统的一个大问题。我们希望提供一个界面，经验丰富的程序员可以使用它来最大限度地提高性能和安全性（能够编写可预测的“正常工作”的游戏代码，而不会出现大量危险和您可以做但更好不做的事情）。因此，像CharacterMoverment这样的东西可能会做一些自定义的事情来最大限度地提高其性能 - 例如，编写模板代码并进行批量更新、扩大范围、将更新循环分成不同的阶段等。我们希望为该用例的异步线程和回滚系统提供一个良好的“低级”接口。在这种情况下 - 角色移动系统本身以自己的方式进行扩展仍然是合理的。例如，提供一种绘制自定义移动模式的方法并提供线程安全的蓝图 API。
>
> 但我们认识到，对于并不真正需要自己的“系统”的简单游戏对象来说，这是不可接受的。我们需要的是更符合虚幻的东西。例如，使用反射系统、具有通用蓝图支持等。有在其他线程上使用蓝图的示例（请参阅 BlueprintThreadSafe 关键字以及动画系统一直在努力的方向）。所以我认为有一天会以某种形式出现。但同样，我们还没有到那一步。
>
> 我知道你只是在问插值，但这是一般的答案：现在我们让你手动完成所有事情，如 NetSerialize、ShouldReconcile、Interpolate 等，但最终我们将有一种方法，就像“如果你只想使用反射系统，则不必手动编写这些东西”。我们只是不想“强迫”每个人都使用反射系统，因为这会带来其他限制，我们认为我们不想在系统的最低级别上采用这些限制。 
>
> 然后，将其与我之前所说的联系起来 - 现在我们真正专注于让一些非常具体的示例工作和高性能，然后我们将注意力转回前端，使事物易于使用和迭代，减少样板文件等供其他人使用。 

**[⬆ 返回顶部](#table-of-contents)**

<a name="changelog"></a>
## 12. GAS 更新记录

This is a list of notable changes (fixes, changes, and new features) to GAS compiled from the official Unreal Engine upgrade changelog and from undocumented changes that I've encountered.如果您发现此处未列出的内容，请提出问题或拉取请求。

<a name="changelog-5.3"></a>
### 5.3
* 崩溃修复：修复了无缝旅行后尝试应用GameplayCue时的崩溃问题。
* 崩溃修复：修复了使用实时编码时由 GlobalAbilityTaskCount 引起的崩溃。
* 崩溃修复：修复了 UAbilityTask::OnDestroy，使其在针对 UAbilityTask_StartAbilityState 等情况递归调用时不会崩溃。
* 错误修复：现在可以安全地在子类中调用 `Super::ActivateAbility`。以前，它会调用 `CommitAbility`。
* 错误修复：添加了对正确复制不同类型的 FGameplayEffectContext 的支持。
* 错误修复：FGameplayEffectContextHandle 现在将在检索“Actors”之前检查数据是否有效。
* 错误修复：保留GameplayAbility系统目标数据位置信息的旋转。
* 错误修复：GameplayAbility系统现在仅在找到有效 PC 时才停止搜索 PC。
* 错误修复：使用现有的 GameplayCueParameters（如果存在）而不是 RemoveGameplayCue_Internal 中的默认参数对象。
* 错误修复：GameplayAbilityWorldReticle 现在面向源 Actor，而不是 TargetingActor。
* 错误修复：如果使用 GiveAbilityAndActivateOnce 传入并且能力列表被锁定，则缓存触发事件数据。
* 错误修复：添加了对 FInheritedGameplayTags 的支持，以立即更新其 CombinedTags，而不是等到保存。
* 错误修复：将 ShouldAbilityRespondToEvent 从仅限客户端的代码路径移至服务器和客户端。
* 错误修复：修复了 FAttributeSetInitterDiscreteLevels 由于曲线简化而无法在烘焙版本中工作的问题。
* 错误修复：在 GameplayAbility 中设置 CurrentEventData。
* 错误修复：确保在可能执行回调之前正确设置 MinimalReplicationTags。
* 错误修复：修复了 ShouldAbilityRespondToEvent 未在实例化 GameplayAbility 上调用的问题。
* 错误修复：禁用 gc.PendingKill 时，在子 Actor 上执行的 Gameplay Cue Notify Actor 不再泄漏内存。
* 错误修复：修复了 GameplayCueManager 中的一个问题，其中 GameplayCueNotify_Actors 可能由于哈希冲突而“丢失”。
* 错误修复：即使我们在 Actor 上没有GameplayTag，WaitGameplayTagQuery 现在也会遵循其查询。
* 错误修复：PostAttributeChange 和 AttributeValueChangeDelegates 现在将具有正确的 OldValue。
* 错误修复：修复了 FGameplayTagQuery 如果该结构是由本机代码创建的，则不会显示正确的查询描述的问题。
* 错误修复：确保在使用能力系统时调用 UAbilitySystemGlobals::InitGlobalData。以前如果用户不调用它，GameplayAbility系统就无法正常运行。
* 错误修复：修复了从 UGameplayAbility::EndAbility 链接/取消链接动画层时的问题。
* 错误修复：更新了AbilitySystemComponent功能，以在使用前检查规格的能力指针。
* 新增内容：向 FGameplayTagRequirements 添加了 GameplayTagQuery 字段，以便可以指定更复杂的要求。
* 新增内容：引入了 FGameplayEffectQuery::SourceAggregateTagQuery 来增强 SourceTagQuery。
* 新增内容：扩展了从控制台命令执行和取消GameplayAbility和GameplayEffect的功能。
* 新增内容：添加了对GameplayAbility蓝图执行“审核”的功能，该蓝图将显示有关如何开发和打算使用它们的信息。
* 更改：现在在主实例上调用 OnAvatarSet，而不是在按 Actor GameplayAbility实例化的 CDO 上调用。
* 更改：允许在同一个GameplayAbility图中同时激活能力和从事件激活能力。
* 更改：AnimTask_PlayMontageAndWait 现在有一个切换功能，允许在 BlendOut 事件后完成和中断。
* 更改：ModMagnitudeCalc 包装函数已声明为 const。
* 更改：FGameplayTagQuery::Matches 现在对于空查询返回 false。
* 更改：更新了 FGameplayAttribute::PostSerialize 以将包含的属性标记为可搜索名称。
* 更改：将 GetAbilitySystemComponent 更新为默认参数 Self。
* 更改：在 SkillTask​​_WaitTargetData 中将函数标记为虚拟函数。
* 更改：删除了未使用的函数 FGameplayAbilityTargetData::AddTargetDataToGameplayCueParameters。
* 更改：删除了残留的 GameplayAbility::SetMovementSyncPoint。
* 更改：从游戏任务和AbilitySystemComponent中删除了未使用的复制标志。
* 更改：将一些GameplayEffect功能移至可选组件中。如有必要，所有现有内容将在 PostCDOCompiled 期间自动更新以使用组件。

https://docs.unrealengine.com/5.3/en-US/unreal-engine-5.3-release-notes/

<a name="changelog-5.2"></a>
### 5.2
* 错误修复：修复了 `UAbilitySystemBlueprintLibrary::MakeSpecHandle` 函数中的崩溃问题。
* 错误修复：修复了GameplayAbility系统中的逻辑，其中非受控 Pawn 将被视为远程，即使它是在服务器本地生成的（例如车辆）。
* 错误修复：正确设置被服务器拒绝的预测实例能力的激活信息。
* 错误修复：修复了可能导致 GameplayCues 卡在远程实例上的错误。
* 错误修复：修复了链接调用 WaitGameplayEvent 时的内存占用问题。
* Bug修复：当多次执行同一节点时，在蓝图中调用AbilitySystemComponent `GetOwnedGameplayTags()`函数不再保留先前调用的返回值。
* 错误修复：修复了 GameplayEffectContext 复制对永远不会复制的动态对象的引用的问题。
  * 这阻止了 GameplayEffect 调用 `Owner->HandleDeferredGameplayCues(this)`，因为 `bHasMoreUnmappedReferences` 始终为 true。
* 新功能：[游戏瞄准系统](https://docs.unrealengine.com/en-US/gameplay-targeting-system-in-unreal-engine/) 是一种创建数据驱动的定位请求的方法。
* 新增内容：添加了对 GameplayTag 查询的自定义序列化支持。
* 新增内容：添加了对复制派生 FGameplayEffectContext 类型的支持。
* 新增内容：资产中的游戏属性现在在保存时注册为可搜索名称，允许在参考查看器中查看对属性的引用。
* 新增内容：为AbilitySystemComponent 添加了一些基本单元测试。
* 新增内容：GameplayAbility系统属性现在尊重核心重定向。这意味着您现在可以在代码中重命名AttributeSet及其属性，并通过向 DefaultEngine.ini 添加重定向条目，将它们正确加载到使用旧名称保存的资产中。
* 更改：允许从代码更改GameplayEffectModifier的评估通道。
* 更改：从GameplayAbility插件中删除了之前未使用的变量 `FGameplayModifierInfo::Magnitude`。
* 更改：删除了AbilitySystemComponent和智能对象实例标签之间的同步逻辑。

https://docs.unrealengine.com/5.2/en-US/unreal-engine-5.2-release-notes/

<a name="changelog-5.1"></a>
### 5.1
* 错误修复：修复了复制的松散GameplayTag未复制给所有者的问题。
* 错误修复：修复了AbilityTask错误，该错误可能会阻止及时进行垃圾收集。
* 错误修复：修复了根据标签监听激活的GameplayAbility无法激活的问题。如果有多个GameplayAbility正在侦听此标签，并且列表中的第一个GameplayAbility无效或无权激活，就会发生这种情况。
* 错误修复：修复了加载警告时正确使用数据注册表的 GameplayEffects，并改进了警告文本。
* 错误修复：从 UGameplayAbility 中删除了代码，该代码错误地仅使用蓝图调试器注册最后一个实例化能力的断点。
* 错误修复：修复了如果在 ApplyGameplayEffectSpecToTarget 内锁定期间调用 EndAbility，GameplayAbility系统能力会卡住的问题。
* 新增内容：添加了对GameplayEffect的支持，以添加阻止的能力标签。
* 新增内容：添加了 WaitGameplayTagQuery 节点。一种基于 UAbilityTask，另一种基于 UAbilityAsync。该节点指定一个 TagQuery，并根据配置在查询变为 true 或 false 时触发其输出引脚。
* 新增功能：修改了控制台变量中的 SkillTask​​ 调试，以在非发布版本中默认启用调试记录和打印（能够根据需要打开/关闭热修复功能）。
* 新增功能：您现在可以将 SkillSystem.AbilityTask.Debug.RecordingEnabled 设置为 0 来禁用，设置为 1 来在非发布版本中启用，设置为 2 来启用所有构建（包括发布）。
* 新增内容：您可以使用AbilitySystem.AbilityTask.Debug.AbilityTaskDebugPrintTopNResults 仅打印日志中的前N 个结果（以避免日志垃圾邮件）。
* 新增功能：STAT_AbilityTaskDebugRecording 可用于测试这些默认调试更改对性能的影响。
* 新增内容：添加了一个调试命令来过滤 GameplayCue 事件。
* 新增内容：向GameplayAbility系统添加了新的调试命令AbilitySystem.DebugAbilityTags、AbilitySystem.DebugBlockedTags 和AbilitySystem.DebugAttribute。
* 新增内容：添加了蓝图函数来获取游戏属性的调试字符串表示形式。
* 新增内容：添加了新的游戏任务资源重叠策略以取消现有任务。
* 更改：现在AbilityTask应确保仅在对能力指针执行所需操作后才调用 Super::OnDestroy，因为调用它后它将被清空。
* 更改：将 FGameplayAbilitySpec/Def::SourceObject 转换为弱引用。
* 更改：将AbilityTask中的AbilitySystemComponent引用设为弱指针，以便垃圾收集可以将其删除。
* 更改：删除了多余的枚举 EWaitGameplayTagQueryAsyncTriggerCondition。
* 更改：GameplayTasksComponent 和AbilitySystemComponent 现在支持注册的子对象API。
* 更改：添加了更好的日志记录以指示游戏功能无法激活的原因。
* 更改：删除了 SkillSystem.Debug.NextTarget 和 PrevTarget 命令，以支持全局 HUD NextDebugTarget 和 PrevDebugTarget 命令。

https://docs.unrealengine.com/5.1/en-US/unreal-engine-5.1-release-notes/

<a name="changelog-5.0"></a>
### 5.0

https://docs.unrealengine.com/5.0/en-US/unreal-engine-5.0-release-notes/

<a name="changelog-4.27"></a>
### 4.27
* 崩溃修复：修复了Root Motion源问题，当 Actor 完成使用带有强度随时间Modifier的恒力Root Motion任务的能力时，联网客户端可能会崩溃。
* 错误修复：修复了使用 GameplayCues 时编辑器加载时间的回归。
* Bug 修复：如果设置相同的 EffectLevel，GameplayEffectsContainer 的 `SetActiveGameplayEffectLevel` 方法将不再弄脏 FastArray。
* 错误修复：修复了 GameplayEffect 混合复制模式中的一个边缘情况，其中未明确由网络连接拥有但使用来自 `GetNetConnection` 的连接的 Actor 将不会收到混合复制更新。
* 错误修复：修复了 GameplayAbility 的类方法 `EndAbility` 中发生的无限递归，该方法是通过从 `K2_OnEndAbility` 再次调用 `EndAbility` 来调用的。
* 错误修复：如果在注册标签之前加载 GameplayTags 蓝图引脚，则它们将不再被静默清除。它们现在与 GameplayTag 变量的工作方式相同，并且可以使用项目设置中的 ClearInvalidTags 选项更改两者的行为。
* 错误修复：改进了 GameplayTag 操作的线程安全性。
* 新增内容：将 SourceObject 暴露给 GameplayAbility 的 `K2_CanActivateAbility` 方法。
* 新内容：本机GameplayTag。引入新的 `FNativeGameplayTag`，使得在加载和卸载模块时正确注册和取消注册的一次性本机标签成为可能。
* 新增内容：更新了 `GiveAbilityAndActivateOnce` 以传入 FGameplayEventData 参数。
* 新增内容：改进了 GameplayAbilities 插件中的 ScalableFloats，以支持从新的数据注册表系统动态查找曲线表。添加了 ScalableFloat 标头，以便更轻松地在功能插件之外重用通用结构。
* 新增内容：添加了通过 GameplayTagsEditorModule 在其他编辑器自定义中使用 GameplayTag UI 的代码支持。
* 新增内容：修改了 UGameplayAbility 的 PreActivate 方法以选择性地接收触发事件数据。
* 新增内容：添加了更多支持，以使用特定于项目的过滤器在编辑器中过滤 GameplayTags。 `OnFilterGameplayTag` 提供引用属性和标签源，因此您可以根据请求标签的资产来过滤标签。
* 新增内容：添加了在初始化后调用 GameplayEffectSpec 的类方法 `SetContext` 时保留原始捕获的 SourceTags 的选项。
* 新增内容：改进了用于从特定插件注册 GameplayTags 的 UI。新的标签 UI 现在允许您为新添加的 GameplayTag 源选择磁盘上的插件位置。
* 新增内容：Sequencer 中添加了一个新轨道，以允许触发使用 GameplayAbiltiySystem 构建的 Actor 上的通知状态。与通知一样，GameplayCueTrack 可以利用基于范围的事件或基于触发器的事件。
* 更改：更改了 GameplayCueInterface 以通过引用传递 GameplayCueParameters 结构。
* 优化：对加载和重新生成 GameplayTag 表进行了多项性能改进，以便优化此选项。

https://docs.unrealengine.com/en-US/WhatsNew/Builds/ReleaseNotes/4_27/

<a name="changelog-4.26"></a>
### 4.26
* GAS 插件不再标记为测试版。
* 崩溃修复：修复了在没有选择有效标签源的情况下添加GameplayTag时发生的崩溃问题。
* 崩溃修复：将路径字符串 arg 添加到消息中以修复 UGameplayCueManager::VerifyNotifyAssetIsInValidPath 中的崩溃。
* 崩溃修复：修复了在使用 ptr 而不检查它时，AbilitySystemComponent_Abilities 中的访问冲突崩溃。
* 错误修复：修复了堆叠 GE 时不会重置所应用效果的其他实例的持续时间的错误。
* 错误修复：修复了导致 CancelAllAbilities 仅取消非实例能力的问题。
* 新增内容：为GameplayAbility提交功能添加了可选标签参数。
* 新增内容：向 PlayMontageAndWait AbilityTask添加了 StartTimeSeconds 并改进了注释。
* 新增内容：向 FGameplayAbilitySpec 添加了标签容器“DynamicAbilityTags”。这些是与规范一起复制的可选能力标签。它们还被应用的GameplayEffect捕获为源标签。
* 新增内容：现在可以从蓝图调用 GameplayAbility IsLocallyControlled 和 HasAuthority 函数。
* 新增内容：如果我们当前正在记录可视记录数据，可视记录器现在将仅收集和存储有关即时 GE 的信息。
* 新增内容：增加了对蓝图节点中游戏属性引脚重定向器的支持。
* 新增内容：添加了新功能，当与Root Motion相关的AbilityTask结束时，它们会将运动组件的运动模式返回到任务开始之前的运动模式。

https://docs.unrealengine.com/en-US/WhatsNew/Builds/ReleaseNotes/4_26/

<a name="changelog-4.25.1"></a>
### 4.25.1
* 固定的！ UE-92787 使用“获取浮动属性”节点保存蓝图时出现崩溃，且属性引脚设置为内联
* 固定的！ UE-92810 实例可编辑GameplayTag属性已内联更改的崩溃生成 Actor

<a name="changelog-4.25"></a>
### 4.25
* 修复了 `RootMotionSource` `AbilityTasks` 的预测
* [`GAMEPLAYATTRIBUTE_REPNOTIFY()`](#concepts-as-attributes) 现在还采用旧的 `Attribute` 值。我们必须将其作为可选参数提供给我们的 `OnRep` 函数。以前，它读取属性值以尝试获取旧值。但是，如果从复制函数调用，则在到达 SetBaseAttributeValueFromReplication 之前旧值已被丢弃，因此我们将获得新值。
* 将 [`NetSecurityPolicy`](#concepts-ga-netsecuritypolicy) 添加到 `UGameplayAbility`。
* 崩溃修复：修复了在没有选择有效标签源的情况下添加GameplayTag时发生的崩溃问题。
* 崩溃修复：删除了攻击者通过能力系统使服务器崩溃的几种方法。
* 崩溃修复：我们现在确保在检查标签要求之前有 GameplayEffect 定义。
* 错误修复：修复了GameplayTag类别不适用于蓝图中的函数参数（如果它们是函数终止符节点的一部分）的问题。
* 错误修复：修复了GameplayEffect标签未通过多个视口复制的问题。
* 错误修复：修复了循环触发能力时，InternalTryActivateAbility 函数可能会使GameplayAbility规范失效的错误。
* 错误修复：更改了我们处理更新标签计数容器内的GameplayTag的方式。当推迟父标签的更新同时删除GameplayTag时，我们现在将在父标签更新后调用与更改相关的委托。这确保了代表广播时标签表处于一致的状态。
* 错误修复：我们现在在确认目标时在内部迭代之前创建生成的目标 actor 数组的副本，因为某些回调可能会修改该数组。
* 错误修复：修复了以下错误：堆叠 GameplayEffects 不会重置正在应用的其他效果实例上的持续时间，并且由调用者设置持续时间，只会为堆栈上的第一个实例正确设置持续时间。堆栈中的所有其他 GE 规范的持续时间均为 1 秒。添加了自动化测试来检测这种情况。
* 错误修复：修复了处理游戏事件委托修改游戏事件委托列表时可能发生的错误。
* 错误修复：修复了导致 GiveAbilityAndActivateOnce 行为不一致的错误。
* 错误修复：重新排序 FGameplayEffectSpec::Initialize 中的一些操作以处理潜在的排序依赖性。
* 新增内容：UGameplayAbility 现在具有 OnRemoveAbility 函数。它遵循与 OnGiveAbility 相同的模式，并且仅在能力的主实例或类默认对象上调用。
* 新增内容：显示被阻止的能力标签时，调试文本现在包括被阻止的标签总数。
* 新增内容：将 UAbilitySystemComponent::InternalServerTryActiveAbility 重命名为 UAbilitySystemComponent::InternalServerTryActivateAbility。调用InternalServerTryActiveAbility 的代码现在应该调用InternalServerTryActivateAbility。
* 新增内容：添加或删除标签时，继续使用过滤文本来显示GameplayTag。先前的行为清除了过滤器。
* 新增：当我们在编辑器中添加新标签时，不要重置标签源。
* 新增内容：添加了查询AbilitySystemComponent以获取具有一组指定标签的所有活动GameplayEffect的功能。新函数称为 GetActiveEffectsWithAllTags，可以通过代码或蓝图访问。
* 新增内容：当与Root Motion相关的AbilityTask结束时，它们现在将运动组件的运动模式返回到任务开始之前的运动模式。
* 新增内容：使 SpawnedAttributes 变得短暂，因此它不会保存可能变得陈旧和不正确的数据。添加了空检查以防止传播任何当前保存的陈旧数据。这可以防止与存储在 SpawnedAttributes 中的不良数据相关的问题。
* API 更改：AddDefaultSubobjectSet 已被弃用。应改用 AddAttributeSetSubobject。
* 新增内容：GameplayAbility现在可以指定要在其上播放Montage的动画实例。

https://docs.unrealengine.com/en-US/WhatsNew/Builds/ReleaseNotes/4_25/

<a name="changelog-4.24"></a>
### 4.24
* 修复了编译时蓝图节点 `Attribute` 变量重置为 `None` 的问题。
* 需要调用 [`UAbilitySystemGlobals::InitGlobalData()`](#concepts-asg-initglobaldata) 才能使用 [`TargetData`](#concepts-targeting-data)，否则您将收到 `ScriptStructCache` 错误，并且客户端将与服务器断开连接。我的建议是现在在每个项目中始终调用它，而在 4.24 之前它是可选的。
* 修复了将 `GameplayTag` setter 复制到之前没有定义变量的蓝图时发生的崩溃。
* `UGameplayAbility::MontageStop()` 函数现在可以正确使用 `OverrideBlendOutTime` 参数。
* 修复了编辑时组件上的 `GameplayTag` 查询变量未被修改的问题。
* 添加了 `GameplayEffectExecutionCalculations` 支持针对不需要属性捕获支持的“临时变量”的作用域修饰符的功能。
  * 实现基本上允许创建 `GameplayTag` 标识的聚合器，作为执行公开要使用作用域修饰符操作的临时值的一种手段；您现在可以构建需要可操作值的公式，而无需从源或目标捕获这些值。
  * 要使用，执行必须向新成员变量 `ValidTransientAggregatorIdentifiers` 添加标记；这些标签将显示在底部范围 mod 的计算修饰符数组中，标记为临时变量 - 相应地更新详细信息自定义以支持功能
* 添加了有限的标签生活质量改进。删除了受限 `GameplayTag` 源的默认选项。添加受限标签时，我们不再重置源，以便更轻松地连续添加多个标签。 
* `APawn::PossessedBy()` 现在将 `Pawn` 的所有者设置为新的 `Controller`。很有用，因为如果 `ASC` 位于 `Pawn` 上，则 [混合复制模式](#concepts-asc-rm) 预计 `Pawn` 的所有者是 `Controller`。
* 修复了 `FAttributeSetInitterDiscreteLevels` 中 POD（普通旧数据）的错误。

https://docs.unrealengine.com/en-US/WhatsNew/Builds/ReleaseNotes/4_24/

**[⬆ 返回顶部](#table-of-contents)**
