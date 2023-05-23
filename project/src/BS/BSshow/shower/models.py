from django.db import models

# Create your models here.

class SensorData(models.Model):
    a_angle = models.IntegerField()
    R_radiate = models.IntegerField()
    S_radiate = models.IntegerField()
    D_radiate = models.IntegerField()
    b_angle = models.IntegerField()
    # 定义其他字段