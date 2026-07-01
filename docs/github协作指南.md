# GitHub 小组协作指南

本文档用于帮助小组成员完成“个人待办事项管理系统”的 GitHub 协同开发。

## 一、第一次加入项目

### 1. 注册 GitHub 账号

每位成员需要先注册 GitHub 账号，并把用户名发给组长。

### 2. 组长邀请成员

组长进入 GitHub 仓库页面：

1. 点击 `Settings`。
2. 点击 `Collaborators`。
3. 点击 `Add people`。
4. 输入组员 GitHub 用户名并发送邀请。
5. 组员在邮箱或 GitHub 通知中接受邀请。

### 3. 克隆仓库到本地

在电脑上选择一个合适的文件夹，打开终端或 Git Bash，执行：

```bash
git clone 仓库地址
```

例如：

```bash
git clone https://github.com/用户名/Personal-To-Do-List-Management-System.git
```

进入项目目录：

```bash
cd Personal-To-Do-List-Management-System
```

## 二、每天开始写代码前

每次开始修改代码前，都要先拉取最新代码：

```bash
git pull
```

这样可以避免本地代码落后，减少冲突。

## 三、推荐分支协作方式

每个人新建自己的功能分支，不直接在 `main` 分支上写代码。

### 1. 创建并切换到自己的分支

```bash
git checkout -b feature/成员姓名-功能名称
```

示例：

```bash
git checkout -b feature/zhangsan-ui
```

### 2. 查看当前分支

```bash
git branch
```

带 `*` 的就是当前所在分支。

## 四、提交自己的修改

### 1. 查看修改状态

```bash
git status
```

### 2. 添加修改文件

```bash
git add .
```

如果只想添加某个文件：

```bash
git add src/ui.cpp
```

### 3. 提交修改

```bash
git commit -m "完成菜单界面设计"
```

提交信息要简洁明确，建议格式：

```text
完成 xxx 功能
修复 xxx 问题
补充 xxx 文档
```

## 五、上传到 GitHub

第一次推送自己的分支：

```bash
git push -u origin feature/成员姓名-功能名称
```

之后继续推送同一个分支：

```bash
git push
```

## 六、发起 Pull Request

代码上传后，在 GitHub 仓库页面会看到 `Compare & pull request` 按钮。

操作步骤：

1. 点击 `Compare & pull request`。
2. 标题写清楚完成了什么功能。
3. 描述中说明修改了哪些文件、实现了哪些功能。
4. 点击 `Create pull request`。
5. 组长检查代码后点击合并。

## 七、合并前检查

每次合并前至少检查：

- 程序是否能正常编译运行。
- 是否只修改了自己负责的功能文件。
- 是否没有提交 `.exe`、`.o` 等编译产物。
- 菜单功能是否能正常进入和退出。
- 数据文件是否能正常保存和读取。

## 八、常见问题处理

### 1. 忘记先 pull，出现冲突怎么办

先不要乱删代码，把冲突文件发到群里，和负责该文件的同学一起处理。

冲突文件中一般会出现：

```text
<<<<<<< HEAD
本地代码
=======
别人提交的代码
>>>>>>> 分支名
```

处理方法：

1. 保留正确代码。
2. 删除 `<<<<<<<`、`=======`、`>>>>>>>` 这些标记。
3. 重新编译测试。
4. 再执行：

```bash
git add .
git commit -m "解决代码冲突"
git push
```

### 2. 提交错文件怎么办

如果只是多提交了 `.exe` 文件，可以先删除它，然后重新提交：

```bash
git rm PersonalToDoList.exe
git commit -m "删除不需要提交的编译文件"
git push
```

### 3. 不会写 Git 命令怎么办

可以使用 GitHub Desktop 图形化工具，但仍然要遵守：

- 开始前先 Pull。
- 修改后写清楚提交信息。
- 上传后发起 Pull Request。
- 不直接改别人负责的文件。

## 九、小组协作建议

- 每天在群里同步一次进度。
- 写代码前先说明今天准备改哪个文件。
- 遇到编译错误先截图，并说明自己改了哪里。
- 组长每次合并后通知大家重新 `git pull`。
- 文档负责人要及时记录已经完成的模块和使用方法。

