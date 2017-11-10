true_predicates = [0] * 3
false_predicates = [0] * 3

if a < b:
	true_predicates[0] += 1
else:
	false_predicates[0] += 1
if a < c:
	true_predicates[1] += 1
else:
	false_predicates[1] += 1
if b < c:
	true_predicates[2] += 1
else:
	false_predicates[2] += 1
