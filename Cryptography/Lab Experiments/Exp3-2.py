
# Question 2
print("Question 2 ============")
print("Hello World")



# # Question 3
print("\nQuestion 3 ============")
a = int(input("Enter a number (a): "))
b = int(input("Enter a number (b) and b should not be 0:"))
if b == 0:
    print("Enter the value again")
    b = int(input("Enter a number (b) and b should not be 0:"))

if a%b == 0:
    print("This is true -  b | a")
else:
    print("This is false - b | a")



# Question 4 - TO FIND GCD (MAX d)
print("\nQuestion 4 ============")
n1 = int(input("Enter a: "))
n2 = int(input("Enter b: "))

minim = min(n1,n2)
maxim = 0
if n1 == 0:
    maxim = n2
if n2 == 0:
    maxim = n1
for i in range(1,minim+1):
    if n1%i == 0 and n2%i == 0:
        d = i
        maxim = max(d,maxim)

print(maxim)




# Question 5 - TO FIND GCD BY EUCLID
print("\nQuestion 5 ============")
def gcd_recursive(a,b):
    if b == 0:
        return a
    else:
        return gcd_recursive(b, a % b)

m1 = int(input("Enter a number (a): "))
m2 = int(input("Enter a number (b): "))  
num = gcd_recursive(m1, m2)
print(num)