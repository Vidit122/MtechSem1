# for i in range(0,6):
#     print(i)

# z = int(2)
# y = int(3.5)
# x = float(2)
# print(x,y,z)

# a = "Vidit Gala"
# print(len(a))
# print(a[0:3])
# print(type(a))

# b = 5
# print(type(b))




# def gcd_recursive(a,b):
#     # print("Greatest Common Divisor!")

#     if b == 0:
#             return a
#     else:
#             return gcd_recursive(b, a % b)


# def Calculator():
#     print("Welcome to Calculator")
#     a = int(input("Enter the 1st number: "))
#     b = int(input("Enter the 2nd number: "))
#     opt = 0
#     while(opt!=5):
#         opt = int(input("Enter 1 to perform add, 2 for subtract, 3 for multiply, 4 for division, and 5 to exit: "))
#         match opt:
#             case 1: print(a+b)
#             case 2: print(a-b)
#             case 3: print(a*b)
#             case 4: print(a/b)
#     print("Thank You!")


# choice = int(input("Enter 1 for GCD, Enter 2 for Calculator: "))
# match choice:
#     case 1: 
#             x = int(input("Enter the 1st number: "))
#             y = int(input("Enter the 2nd number: "))
#             res = gcd_recursive(x,y)
#             print(res)
#     case 2:
#             Calculator() 
#         opt = int(input("Enter 1 to perform add, 2 for subtract, 3 for multiply, 4 for division, and 5 to exit: "))
#         match opt:
#             case 1: print(a+b)
#             case 2: print(a-b)
#             case 3: print(a*b)
#             case 4: print(a/b)
#     print("Thank You!")


# choice = int(input("Enter 1 for GCD, Enter 2 for Calculator: "))
# match choice:
#     case 1: 
#             x = int(input("Enter the 1st number: "))
#             y = int(input("Enter the 2nd number: "))
#             res = gcd_recursive(x,y)
#             print(res)
#     case 2:
#             Calculator() 



# n = int(input("Enter the value of n: "))
# for i in range(1,n+1):
#       for j in range(0,i):
#         print("*", end = "")
#       print()
# for i in range(n-1,0,-1):
#       for j in range(i,0,-1):
#         print("*", end = "")
#       print()


# n = int(input("Enter the value of n: "))
# for i in range(1,n+1):
#       for j in range(0,i):
#         print("*", end = "")
#       print()
# for i in range(n,0,-1):
#       for j in range(i,0,-1):
#         print("*", end = "")
#       print()


a = []
for i in range(5):
    b = int(input("Enter a value: "))
    a.append(b)

print(a)

num = int(input("Enter the number to find: "))

if num in a:
        print("Number found!")
        print("Square of the number is", num*num)
else:
        print("number not found!")

        