# -*- coding: utf-8 -*-
# Generated by Django 1.10.6 on 2017-04-04 12:08
from __future__ import unicode_literals

from django.conf import settings
from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('FlightBooking', '0004_auto_20170404_1205'),
    ]

    operations = [
        migrations.AlterField(
            model_name='flight',
            name='user',
            field=models.ManyToManyField(default=1, to=settings.AUTH_USER_MODEL),
        ),
    ]
