from datetime import datetime
import pytz


utc_tz = pytz.timezone('UTC')
china_tz = pytz.timezone('Asia/Shanghai')
local_naive = datetime.now(tz=utc_tz)
print(local_naive)
local_aware = datetime.now()  # 默认tz=None
print(local_aware)

local_aware_to_naive = datetime.now(tz=china_tz)  # 默认tz=None
print(local_aware_to_naive)
if local_aware_to_naive > local_naive:
    print('yes')


mySet = set()
if len(mySet) == 0:
    print('hahh')