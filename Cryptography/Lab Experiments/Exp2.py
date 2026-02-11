# print("Hello World")


# 1

scores = {"Alice": 88, "Bob": 95, "Charlie": 70, "David": 95, "Eve": 88}  # students with highest marks to find
high = max(scores.values())
print(high)
st = [name for name, sc in scores.items() if sc == high]
# if(scores.values == high):
print(st)


# 2

nums = [1,2,3,4,2,5,6,3]  # check if list has duplicate elements only using a set
s = set()
for i in range(len(nums)):
    s.add(nums[i])
print(s)
if len(s)<len(nums):
    check = True
else:
    check = False
print(check)


# 3

# symmetric diff
a = {1,2,3,4,5}
b = {4,5,6,7,8}

diff = a.union(b) - a.intersection(b)
print(diff)


# 4

list_nums = [2,3,5,2,3,7,11,5,2]  # top 2 most freq elems
maxim = list_nums.count(list_nums[0])
maxis = 0
for i in range(len(list_nums)): 
    n = list_nums.count(list_nums[i])
    if n>maxim:
        maxim = n
        ind = i
    elif n<maxim and n>maxis:
        maxis = n
        index = i
print(f"1st Frequency->", maxim, "1st Elem->", list_nums[i])
print(f"2nd Frequency->", maxis, "2nd Elem->", list_nums[index])


# 5

flat_nums = [1,2,3,[4,5],(6,7),[8,[9,10]]]
lit = []

# print(len(flat_nums[5]))
def flatt(flat_nums):
    for i in range(len(flat_nums)):
        if type(flat_nums[i]) == list or type(flat_nums[i]) == tuple:
            flatt(flat_nums[i])
        else:
            lit.append(flat_nums[i])
flatt(flat_nums)
print(lit)


# 6

flat_nums = [1,2,3,4,5,6,7,8,9,10]
lit = []

def sum(flat_nums):
    sum=0
    for i in range(len(flat_nums)):
        sum = sum + flat_nums[i]
    return sum

# print(len(flat_nums[5]))
def flatt(flat_nums):
    s = sum(flat_nums)
    print(s)

flatt(flat_nums)


# 7

li = [(1,2),(3,4),(5,6)]
tu = ()
new = []
# print(len(li))
for i in range(len(li)):
    s = sum(li[i])
    # print(s)
    new.append(s)

# print(new) 
tu = tuple(new)
print(tu) 


# 9

list_int = [1,2,3,4,5,1,2]
li=[]
s = set()
def exact(list_int):
    for i in range(len(list_int)):
        if list_int.count(list_int[i]) == 2:
            s.add(list_int[i])

# exact(list_int)
print(s)
li = list(s)
print(li)


# 8

words = ["flower", "flow", "flight", "app", "banana"]
new_li = []
for i in range(len(words)):
    for j in range(i+1, len(words)):
        if set(words[i]) & set(words[j]):
            print(words[i], words[j])




# 10

import numpy as np

arr= np.random.rand(5, 5)
arr_int = np.random.randint(0, 10, size=(5, 5))
print(arr_int)
sum=0
# s = sum(arr_int)
# print(s)
for i in range(5):
    for j in range(5):
        sum = sum + arr_int[i][j]

print(sum/25)
for i in range(5):
    for j in range(5):
        if arr_int[i][j] > (sum/25):
            arr_int[i][j] = 1
        else:
            arr_int[i][j] = 0

print(arr_int)
