# -*- coding: utf-8 -*-
# Generated by Django 1.11.20 on 2019-06-03 09:35
from __future__ import unicode_literals

from django.conf import settings
from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        migrations.swappable_dependency(settings.AUTH_USER_MODEL),
        ('FlightBooking', '0007_passengerbills'),
    ]

    operations = [
        migrations.CreateModel(
            name='TicketNotice',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('notice_time', models.DateTimeField(null=True)),
                ('name', models.CharField(max_length=100)),
                ('leave_airport', models.CharField(max_length=100, null=True)),
                ('arrive_airport', models.CharField(max_length=100, null=True)),
                ('flight_id', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, to='FlightBooking.Flight')),
                ('user', models.ManyToManyField(default=1, to=settings.AUTH_USER_MODEL)),
            ],
        ),
    ]