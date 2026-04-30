export LD_LIBRARY_PATH=/usr/share/cix/lib/onnxruntime:$LD_LIBRARY_PATH
export OPERATOR_PATH=/usr/share/cix/lib/onnxruntime/operator
unset DISPLAY

cd /root/ai_model_hub_25_Q3/models/ComputeVision/Image_Classification/onnx_resnet_v1_50

python3 inference_npu.py --images test_data --model_path resnet_v1_50.cix
