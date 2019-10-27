import argparse

parser = argparse.ArgumentParser(description='A tutorial of argparse!')
parser.add_argument("-n", default=10, help="This is the 'n' variable")
args = parser.parse_args()
n = args.n

f1 = open("results/" + n + ".txt", "r")
f2 = open("results/1stmillion.txt", "r")

n = int(n)

primes1 = []
for x in f1.read().split():
	primes1.append(int(x))

primes2 = []
for x in f2.read().split():
	primes2.append(int(x))

i = 0
msg = "Success"
for i in range(len(primes1)):
	if primes1[i] != primes2[i] or primes1[i] > n:
		msg = "Error: primes[" + str(i) + "]=" + str(primes1[i])
		break

if msg == "Success":
	i += 1
	if len(primes2) >= i and primes2[i] <= n:
		msg = "Error: prime " + str(primes2[i]) + " is missing"

print(msg)
