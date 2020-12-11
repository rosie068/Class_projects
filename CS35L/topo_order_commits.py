# Implemented by Rosemary He
# Given a git repository, the commits can be thought of as having the structure of a
# directed acyclic graph (DAG) with the commits being the vertices.
# In particular, one can create a directed edge from each child commit to each of its parent commits.
# Alternatively, one can create a directed edge from each parent to each of its children.
# Note that if a commit is a merge commit, it will have two or even more parents.
# In that case, one has to consider all parents.
# This script locates all commits in a .git repo and outputs a list of topological sorted list
# 
# Keep the function signature,
# but replace its body with your implementation.
#
# Note that this is the driver function.
# Please write a well-structured implemention by
# creating other functions outside of this one,
# each of which has a designated purpose.
#
# As a good programming practice,
# please do not use any script-level variables
# that are modifiable.
# This is because those variables live on forever
# once the script is imported,
# and the changes to them will persist across different
# invocations of the imported functions.

# !/usr/local/cs/bin/python3

import os
import zlib


def topo_order_commits():
    git_dir = find_dir(os.getcwd())
    head_path = os.path.join(git_dir, ".git/refs/heads")
    branch_list = find_branches(head_path)
    for i in range(len(branch_list)):
        branch_list[i] = branch_list[i][len(head_path)+1:]

    hash_list = []
    head_to_branches = {}
    for branch in branch_list:
        bpath = os.path.join(head_path, branch)
        f = open(bpath, 'r')
        hash = f.read().rstrip()
        hash_list.append(hash)
        if hash in head_to_branches.keys():
            head_to_branches[hash].append(branch)
        else:
            head_to_branches.update({hash: [branch]})
    commit_nodes, root_hashes = build_commit_graph(git_dir, hash_list)
    order = get_topo_ordered_commits(commit_nodes, root_hashes)
    print_topo_ordered_commits_with_branch_names(commit_nodes,
                                                 order, head_to_branches)


# find path that contains the .git
def find_dir(path):
    if path == "/":
        print("Not inside a git repository")
        exit(1)

    # look for the top level git dir, if not go up
    while ".git" not in os.listdir(path):
        path = os.path.dirname(path)
        if path == "/":
            print("Not inside a git repository")
            exit(1)
    return path


# find list of full paths to branches
def find_branches(path):
    branch_list = []
    for(dirpath, dirnames, filenames) in os.walk(path):
        for dirs in dirnames:
            sub_branch = find_branches(dirs)
            for subs in sub_branch:
                branch_list.append(os.path.join(dirpath, subs))
        for files in filenames:
            branch_list.append(os.path.join(dirpath, files))
    return branch_list


# return commit_nodes: a dict of {hashid: CommitNode})
# return root_hashes: list of root hashes with no parents
def build_commit_graph(git_dir, local_branch_heads):
    commit_nodes = {}
    root_hashes = []
    visited = set()

    stack = local_branch_heads
    while stack:
        # get next element from stack to store in hash, remove from stack
        commit_hash = stack.pop()

        if commit_hash in visited:
            continue
        visited.add(commit_hash)

        commit = CommitNode(commit_hash)
        if commit_hash not in commit_nodes.keys():
            commit_nodes.update({commit_hash: commit})
        else:
            commit = commit_nodes[commit_hash]


        hash_dir = os.path.join(git_dir, ".git/objects",
                                commit_hash[:2], commit_hash[2:])
        blob = open(hash_dir, 'rb')
        decompressed = zlib.decompress(blob.read()).decode()

        for line in decompressed.splitlines():
            if "parent" in line:
                parent_hash = line.split()[1]
                commit.parents.add(parent_hash)

        if not commit.parents:
            root_hashes.append(commit.commit_hash)

        for p in commit.parents:
            if p not in visited:
                stack.append(p)
            # if p not in commit_nodes.keys():
            if p not in commit_nodes.keys():
                pnode = CommitNode(p)
                commit_nodes.update({p: pnode})

            # record commit_hash is a child of commit node p
            commit_nodes[p].children.add(commit.commit_hash)

    return commit_nodes, root_hashes


# topological commit
def get_topo_ordered_commits(commit_nodes, root_hashes):
    order = []
    visited = set()
    temp_stack = []  # for the current branch we are on
    stack = sorted(root_hashes)

    while stack:
        v = stack.pop()
        # do something here if v already visited
        if v in visited:
            continue
        visited.add(v)

        while temp_stack and \
                v not in commit_nodes[temp_stack[-1]].children:
            g = temp_stack.pop()
            order.append(g)
        temp_stack.append(v)

        # if commit_nodes[v].children:
        for c in sorted(commit_nodes[v].children):
            # do something if c already visited
            if c in visited:
                continue
            stack.append(c)

    # add rest of temp_stack to the order
    while temp_stack:
        lastnodes = temp_stack.pop()
        order.append(lastnodes)

    return order


# commit nodes is a dict where values are commit nodes
# key = commit hash, value = node
# topo_ordered_commits is list of topo ordered commit hashes
# dict where values are list of branch names
# key = commit hash, value = branch name
def print_topo_ordered_commits_with_branch_names(
        commit_nodes, order, head_to_branches):
    jumped = False
    for i in range(len(order)):
        commit_hash = order[i]
        if jumped:
            jumped = False
            sticky_hash = ' '.join(commit_nodes[commit_hash].children)
            print(f'={sticky_hash}')

        branches = sorted(head_to_branches[commit_hash]) \
            if commit_hash in head_to_branches else []
        print(commit_hash + (' ' + ' '.join(branches) if branches else ''))

        if i+1 < len(order) and order[i+1] not in \
                commit_nodes[commit_hash].parents:
            jumped = True
            sticky_hash = ' '.join(commit_nodes[commit_hash].parents)
            print(f'{sticky_hash}=\n')


class CommitNode:
    def __init__(self, commit_hash):
        """
        :type commit_hash: str
        """
        self.commit_hash = commit_hash
        self.parents = set()
        self.children = set()


if __name__ == '__main__':
    topo_order_commits()
