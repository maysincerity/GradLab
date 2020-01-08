from django.shortcuts import render
from django.http import HttpResponseRedirect
from django.views.decorators.csrf import csrf_exempt
from django.contrib.auth import authenticate, login, logout
from .forms import PassengerInfoForm, UserForm
from .models import Flight, FlightSeats, PassengerBills, TicketNotice
from django.contrib.auth.models import User
import datetime
from operator import attrgetter

admin_user_id = 8


class MyIncomeList:
    timeStore = '00'
    income = 0

    def __init__(self, timeStore, income):
        self.timeStore = timeStore
        self.income = income


class Order:
    passenger_name = ''
    flight_name = ''
    flight_route = ''
    flight_leavetime = ''
    flight_price = ''

    def __init__(self, pname, fname, froute, fleavetime, fprice):
        self.passenger_name = pname
        self.flight_name = fname
        self.flight_route = froute
        self.flight_leavetime = fleavetime
        self.flight_price = fprice


# 显示用户订单
def user_order(request):
    if request.user.is_authenticated():
        # 如果用户是管理员，render公司航班收入统计信息页面 finance_report
        if request.user.id == admin_user_id:
            context = finance_report(request)  # 获取要传入前端的数据
            return render(request, 'FlightBooking/finance_report.html', context)
        # 如果用户是普通用户，render用户的机票信息 user_order
        else:
            booked_flights = Flight.objects.filter(user=request.user)  # 从 booksystem_flight_user 表过滤出该用户订的航班
            context = {
                'booked_flights': booked_flights,
                'username': request.user.username,  # 导航栏信息更新
            }
            return render(request, 'FlightBooking/user_order.html', context)
    return render(request, 'FlightBooking/login.html')  # 用户如果没登录，render登录页面


# 显示取票通知
def user_notice(request):
    if request.user.is_authenticated():
        # 如果用户是管理员，render公司航班收入统计信息页面 finance_report
        if request.user.id == admin_user_id:
            context = finance_report(request)  # 获取要传入前端的数据
            return render(request, 'FlightBooking/finance_report.html', context)
        # 如果用户是普通用户，render用户的机票信息 user_order
        else:
            book_ticket_notice = TicketNotice.objects.filter(user=request.user)  # 从 booksystem_flight_user 表过滤出该用户订的航班
            context = {
                'book_ticket_notice': book_ticket_notice,
                'username': request.user.username,  # 导航栏信息更新
                'username1': request.user.username, # 通知上名字
            }
            return render(request, 'FlightBooking/user_notice.html', context)
    return render(request, 'FlightBooking/login.html')  # 用户如果没登录，render登录页面


# 主页
def index(request):
    return render(request, 'FlightBooking/index.html')


# 免除csrf
@csrf_exempt
def book_ticket(request, flight_id):
    if not request.user.is_authenticated():
        return render(request, 'FlightBooking/login.html')
    else:
        flight = Flight.objects.get(pk=flight_id)
        flightseats_tuple = FlightSeats.objects.get_or_create(flight_id_id=flight_id)
        flightseats = flightseats_tuple[0]
        userbills_tuple = PassengerBills.objects.get_or_create(flight_id_id=flight_id)
        userbills = userbills_tuple[0]
        ticket_notice_tuple = TicketNotice.objects.get_or_create(flight_id_id=flight_id)
        ticket_notice = ticket_notice_tuple[0]
        booked_flights = Flight.objects.filter(user=request.user)

        if flight in booked_flights:
            return render(request, 'FlightBooking/book_conflict.html')

        # 不能重复订购
        if request.method == 'POST':
            if flight.capacity > 0:
                flight.book_sum += 1
                flight.capacity -= 1
                flight.income += flight.price
                flight.user.add(request.user)
                flight.save()
                flightseats.seats = flight.book_sum
                flightseats.save()
                userbills.user.add(request.user)
                userbills.bills += flight.price
                userbills.save()
                ticket_notice.notice_time = flight.leave_time + datetime.timedelta(-1)
                ticket_notice.user.add(request.user)
                ticket_notice.name = flight.name
                ticket_notice.leave_airport = flight.leave_airport
                ticket_notice.arrive_airport = flight.arrive_airport
                ticket_notice.save()
        context = {
            'flight': flight,
            'username': request.user.username
        }
        return render(request, 'FlightBooking/book_flight.html', context)


# 退票
def refund_ticket(request, flight_id):
    flight = Flight.objects.get(pk=flight_id)
    flightseats_tuple = FlightSeats.objects.get_or_create(flight_id_id=flight_id)
    flightseats = flightseats_tuple[0]
    userbills_tuple = PassengerBills.objects.get_or_create(flight_id_id=flight_id)
    userbills = userbills_tuple[0]
    ticket_notice_tuple = TicketNotice.objects.get_or_create(flight_id_id=flight_id)
    ticket_notice = ticket_notice_tuple[0]
    flight.book_sum -= 1
    flight.capacity += 1
    flight.income -= flight.price
    flight.user.remove(request.user)
    flight.save()
    flightseats.seats = flight.book_sum
    flightseats.save()
    userbills.bills -= flight.price
    ticket_notice.user.remove(request.user)
    TicketNotice.objects.filter(flight_id_id=flight_id, user=request.user).delete()
    ticket_notice.save()
    return HttpResponseRedirect('/FlightBooking/user_order')


# 登录
def login_user(request):
    if request.method == "POST":
        username = request.POST.get('username', False)
        password = request.POST.get('password', False)
        user = authenticate(username=username, password=password)
        if user is not None:  # 登录成功
            if user.is_active:  # 加载订票页面
                login(request, user)
                context = {
                    'username': request.user.username
                }
                if user.id == admin_user_id:
                    context = finance_report(request)  # 获取要传入前端的数据
                    return render(request, 'FlightBooking/finance_report.html', context)
                else:
                    return render(request, 'FlightBooking/search_result.html', context)
            else:
                return render(request, 'FlightBooking/login.html', {'error_message': 'Your account has been disabled'})
        else:  # 登录失败
            return render(request, 'FlightBooking/login.html', {'error_message': 'Invalid login'})
    return render(request, 'FlightBooking/login.html')


# 注册
def register(request):
    form = UserForm(request.POST or None)
    if form.is_valid():
        user = form.save(commit=False)
        username = form.cleaned_data['username']
        password = form.cleaned_data['password']
        user.set_password(password)
        user.save()
        user = authenticate(username=username, password=password)
        if user is not None:
            if user.is_active:
                login(request, user)
                context = {
                    'username': request.user.username
                }
                return render(request, 'FlightBooking/search_result.html', context)  # 注册成功直接render search_result页面
    context = {
        "form": form,
    }
    return render(request, 'FlightBooking/register.html', context)


# 退出登录
def logout_user(request):
    logout(request)
    form = UserForm(request.POST or None)
    context = {
        "form": form,
    }
    return render(request, 'FlightBooking/login.html', context)


# 搜索结果页面
def search_result(request):
    if request.method == 'POST':
        form = PassengerInfoForm(request.POST)
        if form.is_valid():
            pleave_city = form.cleaned_data.get('leave_city')
            parrive_city = form.cleaned_data.get('arrive_city')
            pleave_date = form.cleaned_data.get('leave_date')
            pleave_time = datetime.datetime.combine(pleave_date, datetime.time())
            # 搜索航班
            flights_a = Flight.objects.filter(leave_city=pleave_city, arrive_city=parrive_city)
            avliable_f = []
            for flight in flights_a:
                flight.leave_time = flight.leave_time.replace(tzinfo=None)
                if flight.leave_time.date() == pleave_time.date():
                    avliable_f.append(flight)
            avliable_f_leave_time = sorted(avliable_f, key=attrgetter('leave_time'))
            avliable_f_arrive_time = sorted(avliable_f, key=attrgetter('arrive_time'), reverse=True)
            avliable_f_price = sorted(avliable_f, key=attrgetter('price'))

            time_format = '%H:%M'

            for flight in avliable_f_price:
                flight.leave_time = flight.leave_time.strftime(time_format)
                flight.arrive_time = flight.arrive_time.strftime(time_format)
            
            dfail = 'none'
            dhead = 'block'
            if len(avliable_f_price) == 0:
                dfail = 'block'
                dhead = 'none'
            context = {
                # 搜多框数据
                'leave_city': pleave_city,
                'arrive_city': parrive_city,
                'leave_date': str(pleave_date),
                # 搜索结果
                'avliable_f_leave_time': avliable_f_leave_time,
                'avliable_f_arrive_time': avliable_f_arrive_time,
                'avliable_f_price': avliable_f_price,
                # 标记
                'dhead': dhead,
                'dfail': dfail
            }
            if request.user.is_authenticated():
                context['username'] = request.user.username
            return render(request, 'FlightBooking/search_result.html', context)
        else:
            return render(request, 'FlightBooking/search_result.html')
    else:
        context = {
            'dhead': 'none',
            'dfail': 'none'
        }
    return render(request, 'FlightBooking/search_result.html', context)


# 管理员后台财务管理
# 统计航空公司每周、每月，每年营业收入情况。
def finance_report(request):
    flights_a = Flight.objects.all()
    flights_a = sorted(flights_a, key=attrgetter('leave_time'))

    per_month_incomes = []
    per_year_incomes = []
    month_set = set()
    year_set = set()
    for flight in flights_a:
        if flight.income > 0:
            this_month = flight.leave_time.strftime('%m')  # datetime获取月
            per_month_incomes.append((this_month, flight.income))  # tuple (month, income)
            month_set.add(this_month)
            this_year = flight.leave_time.strftime('%Y')  # datetime获取年
            per_year_incomes.append((this_year, flight.income))  # tuple (year, income)
            year_set.add(this_year)

    month_incomes = []
    for month in month_set:
        income = sum(per_month_income[1] for per_month_income in per_month_incomes if per_month_income[0] == month)
        month_income = MyIncomeList(month, income)
        month_incomes.append(month_income)
    month_incomes = sorted(month_incomes, key=attrgetter('timeStore'))

    #季度
    period_incomes = []
    income1 = sum(month_income.income for month_income in month_incomes if month_income.timeStore == '01' or month_income.timeStore == '02' or month_income.timeStore == '03')
    income2 = sum(month_income.income for month_income in month_incomes if month_income.timeStore == '04' or month_income.timeStore == '05' or month_income.timeStore == '06')
    income3 = sum(month_income.income for month_income in month_incomes if month_income.timeStore == '07' or month_income.timeStore == '08' or month_income.timeStore == '09')
    income4 = sum(month_income.income for month_income in month_incomes if month_income.timeStore == '10' or month_income.timeStore == '11' or month_income.timeStore == '12')
    period_income1 = MyIncomeList('01', income1)
    period_incomes.append(period_income1)
    period_income2 = MyIncomeList('02', income2)
    period_incomes.append(period_income2)
    period_income3 = MyIncomeList('03', income3)
    period_incomes.append(period_income3)
    period_income4 = MyIncomeList('04', income4)
    period_incomes.append(period_income4)
    period_incomes = sorted(period_incomes, key=attrgetter('timeStore'))

    # 年
    year_incomes = []
    for year in year_set:
        income = sum(per_year_income[1] for per_year_income in per_year_incomes if per_year_income[0] == year)
        year_income = MyIncomeList(year, income)
        year_incomes.append(year_income)
    year_incomes = sorted(year_incomes, key=attrgetter('timeStore'))

    passengers = User.objects.exclude(pk=1)
    order_set = set()
    for passenger in passengers:
        flights = Flight.objects.filter(user=passenger)
        for flight in flights:
            route = flight.leave_city + ' → ' + flight.arrive_city
            order = Order(passenger.username, flight.name, route, flight.leave_time, flight.price)
            order_set.add(order)

    context = {
        'month_incomes': month_incomes,
        'year_incomes': year_incomes,
        'period_incomes': period_incomes,
        'order_set': order_set
    }
    return context

