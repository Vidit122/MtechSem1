# Elliptic Curve Cryptography

from tinyec import registry
import secrets
import random


def compress(publicKey):
    return hex(publicKey.x) + hex(publicKey.y % 2)[2:]

curve = registry.get_curve('brainpoolP256r1')
Ka = secrets.randbelow(curve.field.n)
X = curve.g
Pubx = Ka * curve.g
print(Ka)
print("-----------------------------------------------------------------------------------------")
# print(X)
# print("-----------------------------------------------------------------------------------------")
print("\n")
print("Public key of X is: ", Pubx)
print("-----------------------------------------------------------------------------------------")
print("\n")
# print("X:", compress(Pubx))
# print("-----------------------------------------------------------------------------------------")
# print("\n")
Kb = secrets.randbelow(curve.field.n)
Y = curve.g 
Puby = Kb * curve.g
print(Kb)
print("-----------------------------------------------------------------------------------------")
# print(Y)
# print("-----------------------------------------------------------------------------------------")
print("\n")
print("Public key of Y is: ", Puby)
print("-----------------------------------------------------------------------------------------")
print("\n")
# print("Y:", compress(Puby))
# print("-----------------------------------------------------------------------------------------")
# print("\n")

temp1 = Puby 
temp2 = Pubx 

sharedKa = temp1 * Ka

sharedKb = temp2 * Kb

# print("Shared key of X: ", sharedKa)
# print("\n")
# print("-----------------------------------------------------------------------------------------")
# print("Shared key of Y: ",sharedKb)
# print("\n")
# print("-----------------------------------------------------------------------------------------")
print("\n")

print(compress(sharedKa))
print(compress(sharedKb))

print("\n")
new = secrets.randbelow(curve.field.n)
m = new*curve.g
print("-----------------------------------------------------------------------------------------")
print("\n")
print("Message is =========== ", m)
print("-----------------------------------------------------------------------------------------")
print("\n")
 
k = random.randint(1, 27)
C1 = k*curve.g
C2 = k*Puby + m 

print("Cipher 1 is ======= ", C1)
print("-----------------------------------------------------------------------------------------")
print("Cipher 2 is ======= ", C2)
print("-----------------------------------------------------------------------------------------")
print("-----------------------------------------------------------------------------------------")
print(C2 - C1*Kb)
print("-----------------------------------------------------------------------------------------")
print(m)

