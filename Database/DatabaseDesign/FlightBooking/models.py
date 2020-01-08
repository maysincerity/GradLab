from django.contrib.auth.models import Permission, User
from django.db import models


# Create your models here.
class Flight(models.Model):
    user = models.ManyToManyField(User, default=1)
    name = models.CharField(max_length=100)  # 航班信息
    leave_city = models.CharField(max_length=100, null=True)  # 离开城市
    arrive_city = models.CharField(max_length=100, null=True)  # 到达城市
    leave_airport = models.CharField(max_length=100, null=True)  # 离开机场
    arrive_airport = models.CharField(max_length=100, null=True)  # 到达机场
    leave_time = models.DateTimeField(null=True)  # 离开时间
    arrive_time = models.DateTimeField(null=True) # 到达时间
    capacity = models.IntegerField(default=0, null=True)  # 座位数
    price = models.FloatField(default=0, null=True)  # 价格
    book_sum = models.IntegerField(default=0, null=True)  # 订票人数
    income = models.FloatField(default=0, null=True)  # 收入

    def __str__(self):
        return self.name


class FlightSeats(models.Model):
    flight_id = models.ForeignKey('Flight', on_delete=models.CASCADE)
    seats = models.IntegerField(default=0, null=True)


class PassengerBills(models.Model):
    flight_id = models.ForeignKey('Flight', on_delete=models.CASCADE)
    user = models.ManyToManyField(User, default=1)
    bills = models.FloatField(default=0, null=True)


class TicketNotice(models.Model):
    user = models.ManyToManyField(User, default=1)
    flight_id = models.ForeignKey('Flight', on_delete=models.CASCADE)
    notice_time = models.DateTimeField(null=True)
    name = models.CharField(max_length=100)
    leave_airport = models.CharField(max_length=100, null=True)
    arrive_airport = models.CharField(max_length=100, null=True)
