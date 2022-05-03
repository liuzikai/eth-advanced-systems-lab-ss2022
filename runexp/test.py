import json

with open("/Users/jenny/Desktop/asl-project/team02/data/7a61cb6-GENERATED-1000-5000-500-5-10/5-10-5/config.json", 'r') as f:
	config = json.load(f)
	for a,v in config.items():
		print(a,v)