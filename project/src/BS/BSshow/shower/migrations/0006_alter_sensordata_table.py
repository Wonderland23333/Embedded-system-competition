# Generated by Django 4.2.3 on 2023-07-07 13:47

from django.db import migrations


class Migration(migrations.Migration):
    dependencies = [
        ("shower", "0005_alter_sensordata_humility_alter_sensordata_lighten_and_more"),
    ]

    operations = [
        migrations.AlterModelTable(
            name="sensordata",
            table="sensor",
        ),
    ]
