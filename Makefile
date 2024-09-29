# コンパイラとフラグ
CXX = g++
CXXFLAGS = -Wall -Iinclude -std=c++17 

# フォルダ構成
SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

# サブディレクトリを含むソースファイルとオブジェクトファイル
SRC_FILES = $(wildcard $(SRC_DIR)/*/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

# 実行ファイルの名前
TARGET = MarginX

# デフォルトのターゲット
all: $(TARGET)

# 実行ファイルの作成
$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ_FILES)

# 各オブジェクトファイルのビルド
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)  # オブジェクトファイル用のディレクトリを作成
	$(CXX) $(CXXFLAGS) -c $< -o $@

# クリーンアップ
clean:
	rm -rf $(BUILD_DIR) $(TARGET)
