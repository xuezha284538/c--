#!/bin/bash
# run_demo.sh - 运行示例
set -e

cd "$(dirname "$0")/.."

echo "=================================="
echo " Video Motion Detection Demo"
echo "=================================="
echo ""
echo "选择模式:"
echo "  1) MOG2 模式 + 录制 (推荐)"
echo "  2) 帧差法模式(轻量)"
echo "  3) 自定义参数"
read -p "请输入选项 [1-3]: " choice

case $choice in
    1)
        echo "启动 MOG2 模式 + 录制..."
        ./build/motion_detect 0 1 900 output_$(date +%Y%m%d_%H%M%S).mp4
        ;;
    2)
        echo "启动帧差法模式..."
        ./build/motion_detect 0 0 500
        ;;
    3)
        read -p "摄像头索引 [0]: " cam
        cam=${cam:-0}
        read -p "检测模式 0=帧差法 1=MOG2 [1]: " mode
        mode=${mode:-1}
        read -p "最小面积 [900]: " area
        area=${area:-900}
        read -p "录制文件(留空不录制): " record
        ./build/motion_detect "$cam" "$mode" "$area" "$record"
        ;;
    *)
        echo "无效选项"
        exit 1
        ;;
esac
