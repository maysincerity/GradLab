"""FlightTicket URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/1.10/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  url(r'^$', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  url(r'^$', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.conf.urls import url, include
    2. Add a URL to urlpatterns:  url(r'^blog/', include('blog.urls'))
"""
from django.conf.urls import url
from . import views

app_name = 'FlightBooking'  # 添加这个属性，方便jinja语法

urlpatterns = [
    # 注册与登录
    url(r'^register/$', views.register, name='register'),  # 注册
    url(r'^login_user/$', views.login_user, name='login_user'),  # 登入
    url(r'^logout_user/$', views.logout_user, name='logout_user'),  # 登出

    # 主要页面
    url(r'^$', views.index, name='index'),  # 主页
    url(r'^search_result/$', views.search_result, name='search_result'),  # 搜索结果
    url(r'^user_order/$', views.user_order, name='user_order'),  # 用户信息
    url(r'^user_notice/$', views.user_notice, name='user_notice'),  # 用户信息
    url(r'^book/flight/(?P<flight_id>[0-9]+)/$', views.book_ticket, name='book_ticket'),  # 订票
    url(r'^refund/flight/(?P<flight_id>[0-9]+)/$', views.refund_ticket, name='refund_ticket'),  # 退票
]
