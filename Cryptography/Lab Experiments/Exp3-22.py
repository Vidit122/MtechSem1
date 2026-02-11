# print("\nQuestion 1 ============")
# def gcd_recursive(a,b):
#     if b == 0:
#         return a
#     else:
#         return gcd_recursive(b, a % b)

# m1 = int(input("Enter a number (a): "))
# m2 = int(input("Enter a number (b): "))  
# num = gcd_recursive(m1, m2)
# print(num)



# print("\nQuestion 2 ============")
# n1 = int(input("Enter a number (a): "))

# n2 = int(input("Enter a number (b): "))  
# def gcd(n1,n2):
#     if n1 == 0:
#         return(n2,0,1)
#     else:
#         d,x1,y1 = gcd(n2%n1, n1)
#         x = y1 - (n2//n1)*x1
#         y = x1
#     return(d,x,y)

# d, x, y = gcd(n1,n2)
# print(d,x,y)
# print(n1,"*",x,"(x)","+",n2,"*",y,"(y)","=",d)




# print("\nQuestion 3 ============")
# a = int(input("Enter a number (a): "))
# b = int(input("Enter a number (b): "))  
# k = int(input("Enter a number (k): "))
# ans = 0
# for i in range(a+1, b+1):
#     count = 0
#     for j in range(1,i+1):
#         if i%j == 0:
#             count = count + 1
#     if count == k:
#         ans = ans + 1

# print(ans)



# print("\nQuestion 4 ============")
# a = int(input("Enter a number (a): "))
# b = int(input("Enter a number (b): "))  
# n = int(input("Enter a number (n): "))
# s = set()
# ans = 0
# for i in range(a+1, b+1):
#     count = 0
#     for j in range(1,i+1):
#         if i%j == 0:
#             count = count + 1
#     if count == n:
#         ans = ans + 1
#         s.add(i)

# print(ans)
# print(s)


# print("\nQuestion 5 ============")
# n1 = 85
# n2 = 289
# def gcd(n1,n2):
#     if n1 == 0:
#         return(n2,0,1)
#     else:
#         d,x1,y1 = gcd(n2%n1, n1)
#         x = y1 - (n2//n1)*x1
#         y = x1
#     return(d,x,y)

# d, x, y = gcd(n1,n2)
# print(d,x,y)
# print(n1,"*",x,"(x)","+",n2,"*",y,"(y)","=",d)





# print("\nQuestion 6 ============")
# def gcd_recurs(a,b,k):
#     if b == 0:
#         k = k+1
#         return a, k
#     else:
#         k = k+1
#         return gcd_recurs(b, a % b,k)

# m1 = int(input("Enter a number (a): "))
# m2 = int(input("Enter a number (b): "))  
# k = 0
# num,ans = gcd_recurs(m1, m2,k)
# print(num)
# print("The number of steps are: ", ans)



print("\nQuestion 7 ============")
def gcd_recursive(a,b):
    if b == 0:
        return a
    else:
        return gcd_recursive(b, a % b)

m1 = int(input("Enter a number (a): "))
m2 = int(input("Enter a number (b): "))  
num = gcd_recursive(m1, m2)
if num == 1:
    num1 = gcd_recursive(m1+m2,m1-m2)
    print("GCD of ", m1+m2, "and", m1-m2, "is: ")
    print(num1)
else:
    print("GCD IS NOT 1")