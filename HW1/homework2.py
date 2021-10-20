from collections import defaultdict
import math
from queue import PriorityQueue
import heapq


def actionMapping(x, y, z, actionNum, limitX, limitY, limitZ):
	if actionNum == 1 :
		if(x+1 < limitX):
			return (x+1, y, z)
		return (x,y,z)

	if actionNum == 2 :
		if(x-1 >= 0):
			return (x-1, y, z)
		return (x,y,z)

	if actionNum == 3 :
		if(y+1 < limitY):
			return (x, y+1, z)
		return (x,y,z)

	if actionNum == 4 :
		if(y-1 >= 0):
			return (x, y-1, z)
		return (x,y,z)

	if actionNum == 5 :
		if(z+1 < limitZ):
			return (x, y, z+1)
		return (x,y,z)

	if actionNum == 6 :
		if(z-1 >= 0):
			return (x, y, z-1)
		return (x,y,z)

	if actionNum == 7 :
		if (x+1 < limitX) and (y+1 < limitY):
			return (x+1, y+1, z)
		return (x,y,z)

	if actionNum == 8 :
		if (x+1 < limitX) and (y-1 >=0):
			return (x+1, y-1, z)
		return (x,y,z)

	if actionNum == 9 :
		if (x-1 >= 0) and (y+1 < limitY):
			return (x-1, y+1, z)
		return (x,y,z)

	if actionNum == 10 :
		if (x-1 >=0) and (y-1 >=0):
			return (x-1, y-1, z)
		return (x,y,z)

	if actionNum == 11 :
		if (x+1 < limitX) and (z+1 < limitZ):
			return (x+1, y, z+1)
		return (x,y,z)

	if actionNum == 12 :
		if (x+1 < limitX) and (z-1 >=0):
			return (x+1, y, z-1)
		return (x,y,z)

	if actionNum == 13 :
		if (x-1 >=0) and (z+1 < limitZ):
			return (x-1, y, z+1)
		return (x,y,z)

	if actionNum == 14 :
		if (x-1 >=0) and (z-1 >=0):
			return (x-1, y, z-1)
		return (x,y,z)

	if actionNum == 15 :
		if (y+1 < limitY) and (z+1 < limitZ):
			return (x, y+1, z+1)
		return (x,y,z)

	if actionNum == 16 :
		if(y+1 < limitY) and (z-1 >=0):
			return (x, y+1, z-1)
		return (x,y,z)

	if actionNum == 17 :
		if (y-1 >= 0) and (z+1 <=limitZ):
			return (x, y-1, z+1)
		return (x,y,z)

	if actionNum == 18 :
		if (y-1 >=0) and (z-1 >=0):
			return (x, y-1, z-1)
		return (x,y,z)



def BFS(numNodesWithActions, nodesAndActions, startNode, destinationNode, environmentLimits, parent, visited):
	queue = list()
	queue.append(startNode)
	visited[startNode] = True
	while(queue):
		temp = queue[0]
		queue.pop(0)
		for i in nodesAndActions[(temp[0], temp[1], temp[2])]:
			(x,y,z) = actionMapping(temp[0], temp[1], temp[2], int(i), environmentLimits[0], environmentLimits[1], environmentLimits[2])
			if(x == temp[0] and y == temp[1] and z == temp[2]):
				continue

			if(visited[(x,y,z)] == False):
				parent[(x,y,z)] = temp
				visited[(x,y,z)] = True
				queue.append((x, y, z))

			if(x == destinationNode[0] and y == destinationNode[1] and z == destinationNode[2]):
				path = []
				while((x, y, z) != startNode):
					path.append((x,y,z,1))
					(x,y,z) = parent[(x,y,z)]

				path.append((x,y,z,0))
				return path[::-1]

	return ["FAIL"]



def UCS(numNodesWithActions, nodesAndActions, startNode, destinationNode, environmentLimits, parent, visited):
	queue = []
	costOfNodes = dict()
	costOfNodes[startNode] = 0
	startNode = list(startNode)
	startNode.insert(0,0)
	startNode.insert(0,0)
	startNode = tuple(startNode)
	heapq.heapify(queue)
	heapq.heappush(queue, startNode)
	totalUCSCost = 0
	while(queue):
		temp = heapq.heappop(queue)
		if(visited[temp]):
			continue
		visited[temp] = True
		if(temp[2] == destinationNode[0] and temp[3] == destinationNode[1] and temp[4] == destinationNode[2]):
				path = []
				totalUCSCost = temp[0]
				while(temp!=startNode):
					path.append(temp)
					temp = parent[temp]
				path.append((totalUCSCost, 0, temp[2], temp[3], temp[4]))
				return (path[::-1], totalUCSCost)

		for i in nodesAndActions[(temp[2], temp[3], temp[4])]:
			(x,y,z) = actionMapping(temp[2], temp[3], temp[4], int(i), environmentLimits[0], environmentLimits[1], environmentLimits[2])
			if(x == temp[2] and y == temp[3] and z == temp[4]):
				continue
			cost = 0
			if ((x != temp[2] and y != temp[3]) or (z != temp[4] and y != temp[3]) or (x != temp[2] and z != temp[4])):
				cost=14
			else:
				cost=10
			vertex = (cost + temp[0], cost, x,y,z)
			if(visited[vertex] == False):
				if((x,y,z) in costOfNodes):
					if(costOfNodes[(x,y,z)] > cost + temp[0]):
						costOfNodes[(x,y,z)] = cost + temp[0]
				else:
					costOfNodes[(x,y,z)] = cost + temp[0]
					parent[vertex] = temp
					heapq.heappush(queue, vertex)
			del(vertex)
			del(cost)
	return ["FAIL"], None




def heuristicFunction(startNode, destinationNode):
	val = 0
	val += (startNode[0] - destinationNode[0])**2
	val += (startNode[1] - destinationNode[1])**2
	val += (startNode[2] - destinationNode[2])**2
	return int(math.sqrt(val))
	

def AStarSearch(numNodesWithActions, nodesAndActions, startNode, destinationNode, environmentLimits, parent, visited):
	queue = []
	costOfNodes = dict()
	costOfNodes[startNode] = 0
	startNode = list(startNode)
	
	startNode.insert(0,0)


	h_val_start_node = heuristicFunction( startNode[1:], destinationNode)
	startNode.insert(0,h_val_start_node)
	startNode.append(h_val_start_node)

	print("heuristic startNode: " ,h_val_start_node)

	startNode = tuple(startNode)
	heapq.heapify(queue)
	heapq.heappush(queue, startNode)
	totalAStarCost = 0
	while(queue):
		temp = heapq.heappop(queue)


		temp = list(temp)
		temp[0] = temp[0] - temp[5]
		temp = tuple(temp)

		# print(temp)


		if(visited[temp]):
			continue
		visited[temp] = True
		if(temp[2] == destinationNode[0] and temp[3] == destinationNode[1] and temp[4] == destinationNode[2]):
				path = []
				totalAStarCost = temp[0]

				startNode = list(startNode)
				startNode[0] = 0
				startNode = tuple(startNode)

				while(temp!=startNode):
					path.append(temp)
					# print(temp, " ", parent[temp])
					temp = parent[temp]
				path.append((totalAStarCost, 0, temp[2], temp[3], temp[4]))

				return (path[::-1], totalAStarCost)

		for i in nodesAndActions[(temp[2], temp[3], temp[4])]:
			(x,y,z) = actionMapping(temp[2], temp[3], temp[4], int(i), environmentLimits[0], environmentLimits[1], environmentLimits[2])
			if(x == temp[2] and y == temp[3] and z == temp[4]):
				continue
			cost = 0
			if ((x != temp[2] and y != temp[3]) or (z != temp[4] and y != temp[3]) or (x != temp[2] and z != temp[4])):
				cost=14
			else:
				cost=10
			h_val = heuristicFunction((x,y,z), destinationNode)
			vertex = (cost + temp[0] + h_val, cost, x,y,z, h_val)

			if(visited[vertex] == False):
				if((x,y,z) in costOfNodes):
					if(costOfNodes[(x,y,z)] > cost + temp[0] ):
						costOfNodes[(x,y,z)] = cost + temp[0] 
				else:
					costOfNodes[(x,y,z)] = cost + temp[0] 
					parent[(cost + temp[0], cost, x,y,z, h_val)] = temp
					heapq.heappush(queue, vertex)
			del(vertex)
			del(cost)
	return ["FAIL"],None

	

def main():
	searchTechnique = ""
	nodesAndActions = defaultdict()
	startNode = []
	destinationNode = []
	environmentLimits = []
	numNodesWithActions = 0
	inputFileAsList = []
	parent = defaultdict()
	visited = defaultdict(lambda: False)

	with open('input.txt') as inputFile:
		inputFileAsList = inputFile.read().splitlines()

	inputFileAsList = [entry.split(' ') for entry in inputFileAsList]
	count = 0
	for line in inputFileAsList:
		if count == 0 :
			searchTechnique += line[0]
		if count == 1:
			environmentLimits += line 
		if count == 2:
			startNode += line
		if count == 3:
			destinationNode += line
		if count == 4:
			numNodesWithActions = int(line[0])
		elif count>4 :
			nodesAndActions[(int(line[0]), int(line[1]), int(line[2]))] = line[3:]
		count +=1
	startNode = tuple([int(i) for i in startNode])
	destinationNode = tuple([int(i) for i in destinationNode])
	environmentLimits = tuple([int(i) for i in environmentLimits])


	ansPath = list()
	if(searchTechnique == 'BFS'):
		ansPath = BFS(numNodesWithActions, nodesAndActions, startNode, destinationNode, environmentLimits, parent, visited)
		f = open('output2.txt', 'w+')
		if(ansPath[0]=="FAIL"):
			f.write("FAIL")
			return
		f.write(str(len(ansPath)-1))
		f.write('\n')
		f.write(str(len(ansPath)))
		f.write('\n')
		for i in range(len(ansPath)):
			f.write(str(ansPath[i][0]))
			f.write(' ')
			f.write(str(ansPath[i][1]))
			f.write(' ')
			f.write(str(ansPath[i][2]))
			f.write(' ')
			f.write(str(ansPath[i][3]))
			f.write('\n')
				
	if(searchTechnique == "UCS"):
		ansPath, totalUCSCost = UCS(numNodesWithActions, nodesAndActions, startNode, destinationNode, environmentLimits, parent, visited)
		f = open('output2.txt', 'w+')
		if(ansPath[0]=="FAIL"):
			f.write("FAIL")
			return
		else:
			f.write(str(totalUCSCost))
			f.write('\n')
			f.write(str(len(ansPath)))
			f.write('\n')
			for i in range(len(ansPath)):
				f.write(str(ansPath[i][2]))
				f.write(' ')
				f.write(str(ansPath[i][3]))
				f.write(' ')
				f.write(str(ansPath[i][4]))
				f.write(' ')
				f.write(str(ansPath[i][1]))
				f.write('\n')


	if(searchTechnique == "A*"):
		ansPath, totalAStarCost = AStarSearch(numNodesWithActions, nodesAndActions, startNode, destinationNode, environmentLimits, parent, visited)
		f = open('output2.txt', 'w')
		if(ansPath[0]=="FAIL"):
			f.write("FAIL")
			return
		else:
			f.write(str(totalAStarCost))
			f.write('\n')
			f.write(str(len(ansPath)))
			f.write('\n')
			for i in range(len(ansPath)):
				f.write(str(ansPath[i][2]))
				f.write(' ')
				f.write(str(ansPath[i][3]))
				f.write(' ')
				f.write(str(ansPath[i][4]))
				f.write(' ')
				f.write(str(ansPath[i][1]))
				f.write('\n')



if __name__=="__main__":
	main()

