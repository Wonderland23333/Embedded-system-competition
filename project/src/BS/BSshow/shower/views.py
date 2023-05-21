from django.shortcuts import render
from .models import SensorData
import json

def data_list(request):
    sensor_data_list = SensorData.objects.all()
    return render(request, 'data_list.html', {'sensor_data_list': sensor_data_list})

def pie_chart(request):
    sensor_data_list = SensorData.objects.all()
    return render(request, 'pie_chart.html', {'sensor_data_list': sensor_data_list})

def line_chart(request):
    sensor_data_list = SensorData.objects.all()
    sensor_data_list_json = json.dumps(list(sensor_data_list.values()))
    return render(request, 'line_chart.html', {'sensor_data_list_json': sensor_data_list_json})

