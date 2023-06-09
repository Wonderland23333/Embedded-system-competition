"""BSshow URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/3.2/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path
from shower.views import data_list
from shower.views import pie_chart
from shower.views import line_chart
from shower.views import bar_chart
from django.shortcuts import redirect
from django.conf.urls.static import static
from django.conf import settings 


urlpatterns = [
    path('', lambda request: redirect('admin:index')),
    path('admin/', admin.site.urls),
    path('data_list/', data_list, name='data_list'),
    path('pie_chart/', pie_chart, name='pie_chart'),
    path('line_chart/', line_chart, name='line_chart'),
    path('bar_chart/', bar_chart, name='bar_chart'),
]+ static(settings.STATIC_URL, document_root=settings.STATIC_ROOT)