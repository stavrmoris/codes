uname -r
ls /dev/aipu* 2>/dev/null || echo "no /dev/aipu"
dmesg | grep -Ei "aipu|npu|cix|zhouyi" | tail -50
