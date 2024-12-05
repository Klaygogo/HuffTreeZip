
# 霍夫曼编码压缩/解压缩工具

## 概述

这是一个基于 C++ 实现的霍夫曼编码压缩和解压缩工具。霍夫曼编码是一种无损数据压缩算法，广泛应用于文件压缩、图像压缩等领域。该工具能够读取输入文件并生成压缩后的文件，同时也能从压缩文件中恢复原始文件。

## 功能特点

- **压缩**：将输入文件压缩为二进制文件。
- **解压缩**：从压缩文件中恢复原始文件。
- **高效性**：利用优先队列构建霍夫曼树，确保高效的编码和解码过程。
- **跨平台**：支持多种操作系统（Windows, Linux, macOS），只需编译即可运行。

## 安装与编译

### 依赖

- **C++ 编译器**：如 GCC 或 Clang。
- **标准库**：项目使用 C++ 标准库，无需额外安装。

### 编译步骤

1. **克隆仓库**：
   ```sh
   git clone https://github.com/yourusername/huffman-compressor.git
   cd huffman-compressor
   ```

2. **编译代码**：
   ```sh
   g++ -o huffman_compressor HuffCode.cpp
   ```

3. **运行程序**：
   ```sh
   ./huffman_compressor
   ```

## 使用说明

### 压缩文件

1. **选择操作**：
   ```sh
   请选择操作: 1. 压缩 2. 解压缩
   输入: 1
   ```

2. **输入源文件名**：
   ```sh
   请输入源文件名：example.txt
   ```

3. **输入目标文件名**：
   ```sh
   请输入目标文件：example.huf
   ```

4. **完成提示**：
   ```sh
   这在处理中，请等待………
   处理结束
   ```

### 解压缩文件

1. **选择操作**：
   ```sh
   请选择操作: 1. 压缩 2. 解压缩
   输入: 2
   ```

2. **输入压缩文件名**：
   ```sh
   请输入压缩文件名：example.huf
   ```

3. **输入目标文件名**：
   ```sh
   请输入目标文件：decompressed_example.txt
   ```

4. **完成提示**：
   ```sh
   这在解压中，请等待………
   处理结束
   ```

## 示例

### 压缩示例

假设有一个名为 `sample.txt` 的文件，内容如下：
```
Hello, World!
```

运行压缩命令：
```sh
./huffman_compressor
请选择操作: 1. 压缩 2. 解压缩
输入: 1
请输入源文件名：sample.txt
请输入目标文件：sample.huf
这在处理中，请等待………
处理结束
```

### 解压缩示例

运行解压缩命令：
```sh
./huffman_compressor
请选择操作: 1. 压缩 2. 解压缩
输入: 2
请输入压缩文件名：sample.huf
请输入目标文件：decompressed_sample.txt
这在解压中，请等待………
处理结束
```

验证解压缩结果：
```sh
cat decompressed_sample.txt
Hello, World!
```

## 代码结构

```
huffman-compressor/
├── README.md
└── HuffCode.cpp
```

- **README.md**: 项目文档。
- **HuffCode.cpp**: 包含所有实现代码，包括霍夫曼树的构建、编码和解码逻辑。

## 贡献

欢迎任何形式的贡献，包括但不限于 bug 报告、功能建议和代码改进。请通过以下方式参与：

1. **Fork 项目**：
   ```sh
   git fork https://github.com/yourusername/huffman-compressor.git
   ```

2. **创建分支**：
   ```sh
   git checkout -b feature/your-feature-name
   ```

3. **提交更改**：
   ```sh
   git commit -m "Add your feature"
   ```

4. **推送分支**：
   ```sh
   git push origin feature/your-feature-name
   ```

5. **创建 Pull Request**：
   访问 GitHub 页面并创建 Pull Request。

## 许可证

本项目采用 MIT 许可证。详细信息见 [LICENSE](LICENSE) 文件。

---

该readme由通义千问生成，谨慎参考
