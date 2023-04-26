from django.shortcuts import render

# Create your views here.
from .models import DataItem



def data_list(request):
    data_items = DataItem.objects.all()
    return render(request, 'data_list.html', {'data_items': data_items})
