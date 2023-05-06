from django.shortcuts import render
from .models import SensorData

def data_list(request):
    data_items = SensorData.objects.all()
    return render(request, 'data_list.html', {'data_items': data_items})
