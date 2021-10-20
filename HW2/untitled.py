def bfs(graph, source, visited, parent):
	queue = []
    queue.append(source)
    visited[source] = True
    triosList = list()
    while queue:
        front = queue.pop(0)
        involvedTrio = list()
        for adj in graph[source]:
            if visited[adj] == False:
            	involvedTrio.append(adj)
                queue.append(adj)
                visited[adj] = True
                parent[adj] = front
        for node in involvedTrio:
        	for adj in graph[node]:
        		if(adj!=node and adj in involvedTrio):
        			if (adj, node, front) not in triosList:
        				triosList.append((node, adj, front))
    return triosList



 

def helper(graph, n, source):
	triosList = bfs(graph, source)
	mini = -1
	for value in triosList:
		count = 0
		for adj in graph[value]:
			if adj not in value:
				count+=1
		mini = min(mini, count)
	return mini

def findAns(n, fromList, toList):
	graph = dict()
	visited = dict()
	for i in range(0,len(fromList)):
		graph[fromList[i]].append(toList[i])
		graph[toList[i]].append(fromList[i])
		visited[fromList[i]] = False
		visited[toList[i]] = False
	source = fromList[0]
	parent = dict()
	ans = helper(graph, n, source, visited, parent)
	return ans

	if __name__ == '__main__':
		friends_nodes , edges = map(int, input.rstrip(), split())
		fromList = [0] * edges
		toList = [0] * edges
		for i in range(edges):
			fromList[i], toList[i] = map(int, input().rstrip().split())
		result = findAns(friends_nodes, fromList, toList)
		print(result)

		