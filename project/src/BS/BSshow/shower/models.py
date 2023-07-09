from django.db import models

# Create your models here.

class SensorData(models.Model):
    temperature = models.FloatField()
    humility = models.FloatField()
    lighten = models.FloatField()
    #D_radiate = models.IntegerField()
    #b_angle = models.IntegerField()
    date = models.DateTimeField()
    # 定义其他字段a
    class Meta:
        db_table = 'solar'

class WarningData(models.Model):
    data=models.FloatField()
    date=models.DateTimeField()
    warning=models.TextField()
    # 定义其他字段a
    class Meta:
        db_table = 'warning_list'