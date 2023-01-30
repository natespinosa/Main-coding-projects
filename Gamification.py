
'''Semantic Similarity: This code provides the semantic similarity of any given list of words. It can take in any sentence 
(with any sort of accotations) and give the percetnage of similarity among the words given.

Author of fucntion 'def norm(vec)': Michael Guerzhoy. Last modified: Nov. 14, 2016.
'''

import math

def norm(vec):
    '''Return the norm of a vector stored as a dictionary,
    as described in the handout for Project 3.
    '''

    sum_of_squares = 0.0
    for x in vec:
        sum_of_squares += vec[x] * vec[x]

    return math.sqrt(sum_of_squares)


def cosine_similarity(vec1, vec2):
  res = 0
  for i in vec1:
    for j in vec2:
      if i == j:
        res += vec1[i]*vec2[j]
  return res/(norm(vec1)* norm(vec2))


def build_semantic_descriptors(sentences):
    words = {}
    for i in range(len(sentences)):
        for j in range(len(sentences[i])):
            words.update({sentences[i][j]:{}})        # make one continuous list of all sentences from the input

    semantic_descriptors = {}
    for word in sorted(words):
        dictionary = {}
        # print(word)
        for i in range(len(sentences)):

            if word in sentences[i]:
                for j in range(len(sentences[i])):
                    if sentences[i][j] in dictionary:
                        dictionary[sentences[i][j]] += 1
                    else:
                        dictionary.update({sentences[i][j]:1})
        dictionary.pop(word)
        semantic_descriptors.update({word:dictionary})

    return semantic_descriptors

def build_semantic_descriptors_from_files(filenames):
    working_list_of_lists = []

    for file in range(len(filenames)):
        text = open(filenames[file], "r", encoding="latin1").read()
        sentences = text.replace("!", ".").replace("?", ".").replace("\n"," ").replace("("," ").replace(")"," ").replace("'"," ").lstrip().rstrip().split(".")

        if "" in sentences:
            sentences.remove("")

        for i in range(len(sentences)):
            sentences[i] = sentences[i].lstrip().rstrip()

        for list in sentences:
            sentence = list.replace("-", ",").replace("--", ",").replace(":", ",").replace(";", ",").replace(", ", ",").replace('"'," ").replace("\n"," ").replace("  ", " ").replace(" ", ",").lower()
            sentence = sentence.lstrip().rstrip().split(",")
            working_list_of_lists.append(sentence)

    return build_semantic_descriptors(working_list_of_lists)



def most_similar_word(word, choices, semantic_descriptors, similarity_fn):
    similarity = 0
    word_chosen = " "

    if word not in semantic_descriptors.keys():
        return choices[0]

    dictionary_0 = semantic_descriptors[word]

    for choice in choices:
        if choice in semantic_descriptors.keys():
            dictionary_1 = semantic_descriptors[choice]

            if similarity == 0:
                similarity = similarity_fn(dictionary_0, dictionary_1)
                word_chosen = choice

            if similarity < similarity_fn(dictionary_0, dictionary_1):
                similarity = similarity_fn(dictionary_0, dictionary_1)
                word_chosen = choice

    return word_chosen


def run_similarity_test(filename, semantic_descriptors, similarity_fn):
    text = []
    question_line = 0
    ans = 0

    f = open(filename, encoding="latin1")
    s = f.read()                #read
    s = s.split("\n")               #split by lines
    for i in range(len(s)):         #range number of lines
        text.append(s[i].split(" "))

    for line in range(len(text)):
        question_line += 1

        word = text[line][0]
        correct_answer = text[line][1]
        choices = text[line][2:]

        if most_similar_word(word, choices, semantic_descriptors, cosine_similarity) == correct_answer:
            ans += 1

    percentage = (ans/question_line)*100

    return percentage
