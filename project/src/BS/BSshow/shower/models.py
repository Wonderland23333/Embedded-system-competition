from django.db import models

# Create your models here.

class SensorData(models.Model):
<<<<<<< HEAD
    name = models.CharField(max_length=100)
    value = models.IntegerField()
=======
    a_angle = models.IntegerField()
    R_radiate = models.IntegerField()
    S_radiate = models.IntegerField()
    D_radiate = models.IntegerField()
    b_angle = models.IntegerField()
>>>>>>> 47fcea48c89dbc56470dca9f4b7e6d7840425230
    # 定义其他字段