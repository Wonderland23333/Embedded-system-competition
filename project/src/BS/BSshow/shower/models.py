from django.db import models

# Create your models here.

class SensorData(models.Model):
    name = models.CharField(max_length=100)
    value = models.IntegerField()
    # 定义其他字段