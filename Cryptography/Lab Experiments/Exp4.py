# Diophantine eqns

import math

def extended_gcd(a, b):
    if a == 0:
        return b, 0, 1
    gcd, x1, y1 = extended_gcd(b % a, a)
    x = y1 - (b // a) * x1
    y = x1
    return gcd, x, y

def solve_linear_diophantine(a, b, c):
    gcd, x0, y0 = extended_gcd(a, b)

    if c % gcd != 0:
        return "No integer solutions exist."

    x_1 = x0 * (c // gcd)
    y_1 = y0 * (c // gcd)

    b_p = b // gcd
    a_p = a // gcd

    def get_solution(k):
        x_k = x_1 + k * b_p
        y_k = y_1 - k * a_p
        return x_k, y_k

    return get_solution, gcd


a = 56
b = 72
c = 40

solution_generator, common_divisor = solve_linear_diophantine(a, b, c)

if isinstance(solution_generator, str):
    print(solution_generator)
else:
    print(f"GCD({a}, {b}) = {common_divisor}")
    print("General solutions (x, y) for 56x + 72y = 40:")
    for k in range(-10,10):
        x, y = solution_generator(k)
        print(f"k = {k}: x = {x}, y = {y}")


a = 24
b = 138
c = 18

solution_generator, common_divisor = solve_linear_diophantine(a, b, c)

if isinstance(solution_generator, str):
    print(solution_generator)
else:
    print(f"GCD({a}, {b}) = {common_divisor}")
    print("General solutions (x, y) for 24x + 138y = 18:")
    for k in range(-10,10):
        x, y = solution_generator(k)
        print(f"k = {k}: x = {x}, y = {y}")







print("\nQuestion 2 ============")
def gcd_recursive(a,b):
    if b == 0:
        return a
    else:
        return gcd_recursive(b, a % b)

m1 = int(input("Enter a number (a): "))
m2 = int(input("Enter a number (b): "))  
num = gcd_recursive(m1,m2)
print(num)






print("\nQuestion 3 ============")
a = [6,3]
for i in range(4):
    x = int(input("Enter number:-"))
    a.append(x)
print(a)
def gcd_recursive(a,b):
    if b == 0:
        return a
    else:
        return gcd_recursive(b, a % b)
g = gcd_recursive(a[0],a[1])
d = g
for i in range(2,5):
    d = gcd_recursive(d,a[i])

print(d)





print("\nQuestion 4 ============")
arr = []
n = int(input("Enter value of n: "))
for i in range(n):
    val = int(input("Enter array values: "))
    arr.append(val)
print(arr)
s = ""
for i in range(n):
    v = arr[i]%10
    s = s + str(v)

n = int(s)
print(n)
if n%10 == 0: print("TRUE")
else: print("False")





print("\nQuestion 5 ============")
a = int(input("Enter the value of a: "))
b = int(input("Enter the value of b: "))
count = 0
while a%3!=0 and b%3!=0:
    if a>b:
        a = abs(a-b)
    else:
        b = abs(a-b)
    count = count+1

print("No.of steps = ", count)






print("\nQuestion 6 ============")
a = int(input("Enter the value of a: "))
b = int(input("Enter the value of b: "))
arr = []
for i in range(a,b+1):
    if i%2==0 or i%3==0 or i%5==0:
        arr.append(i)

print(arr)
print(len(arr))





