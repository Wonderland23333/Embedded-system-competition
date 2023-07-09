from django.shortcuts import render
from .models import SensorData, WarningData
from django.core.serializers import serialize
from django.utils import timezone
from django.http import JsonResponse
import json

def data_list(request):
    sensor_data_list = SensorData.objects.all()
    print(sensor_data_list)
    return render(request, 'data_list.html', {'sensor_data_list': sensor_data_list})

def pie_chart(request):
    sensor_data_list = SensorData.objects.all()
    return render(request, 'pie_chart.html', {'sensor_data_list': sensor_data_list})

def line_chart(request):
    sensor_data_list = SensorData.objects.all()
    sensor_data_list_json = json.dumps(list(sensor_data_list.values()))
    return render(request, 'line_chart.html', {'sensor_data_list_json': sensor_data_list_json})

def bar_chart(request):
    sensor_data_list = SensorData.objects.all()
    sensor_data_list_json = json.dumps(list(sensor_data_list.values()))
    return render(request, 'bar_chart.html', {'sensor_data_list_json': sensor_data_list_json})

class DateTimeEncoder(json.JSONEncoder):
    def default(self, obj):
        if isinstance(obj, timezone.datetime):
            return obj.strftime('%Y-%m-%d %H:%M:%S')
        return super().default(obj)

def simple_count(request):
    sensor_data_list = SensorData.objects.all().values()
    sensor_data_list_json = json.dumps(list(sensor_data_list), cls=DateTimeEncoder)
    return render(request, 'simple_count.html', {'sensor_data_list_json': sensor_data_list_json})

def get_sensor_data(request):
    # 获取最新的传感器数据
    sensor_data = SensorData.objects.all().values()  # 替换成你获取最新数据的代码
    sensor_data_list_json = json.dumps(list(sensor_data), cls=DateTimeEncoder)
    json_data = sensor_data_list_json
    # 返回JSON响应
    return JsonResponse(json_data, safe=False)

def get_warning_data(request):
    # 获取最新的传感器数据
    sensor_data_list = WarningData.objects.all()
    print(sensor_data_list)
    return render(request, 'warning_list.html', {'sensor_data_list': sensor_data_list})