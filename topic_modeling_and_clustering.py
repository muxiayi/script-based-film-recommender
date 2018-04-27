from nltk.tokenize import RegexpTokenizer
from nltk.stem.porter import PorterStemmer
from nltk.corpus import stopwords
from sklearn.decomposition import NMF, LatentDirichletAllocation
from sklearn.feature_extraction.text import CountVectorizer, TfidfVectorizer
from sklearn.cluster import KMeans
import collections
import os

def flatten_list(l, a=None):
    """
    Given a list with nested list inside, return a flatten list by recursion.
    """
    if a is None:
        # initialize with empty list
        a = []
    for i in l:
        if isinstance(i, list):
            flatten_list(i, a)
        else:
            a.append(i)
    return a

def nlp_preprocessing(body: list, stemming: bool) -> list:
    """
    Given a list containing the chats cleaned, perform tokenizing, stopword removing and stemming.
    :param body: a list whose items are strings of chats
    :return: a list of processed words
    """
    # remove all numbers and punctuations
    tokenizer = RegexpTokenizer(r'[a-zA-Z]\w+\'?\w*')
    tokens = [tokenizer.tokenize(str(raw).lower()) for raw in body]
    # remove common stopwords
    en_stop = stopwords.words('english')
    stopped_tokens = [[i for i in token if i not in en_stop] for token in tokens]
    # stemming: reduce derived words to the root/stem words, including remove 'ed', 's', 'ing', 'ly', etc.
    # it is highly likely to result in some errors due to the fact of the static method
    if stemming == True:
        p_stemmer = PorterStemmer()
        stemmed_tokens = [[p_stemmer.stem(i) for i in token] for token in stopped_tokens]
    # get the corpus by joining the tokens
        corpus = [' '.join(token) for token in stemmed_tokens]
    else:
        corpus = [' '.join(token) for token in stopped_tokens]
    return corpus

def sklearn_model_get_top_words(model, feature_names, num_top_words: int):
    """
    The function is used with LDA, NMF and other sklearn-based models only.
    Given a model object, feature names and the number of top words, return a list of topic words.
    :param model: LDA or NMF fitted.
    :param feature_names: feature names by either tf_vectorizer or tfidf_vectorizer.
    :param num_top_words: number of words in every topic
    :return: a list of topic words: [['w1', 'w2', ..., 'wn'], ['w1', 'w2', ..., 'wn'], ... ]
    """
    top_word = []
    for topic_idx, topic in enumerate(model.components_):
        topic_top_word = [feature_names[i] for i in topic.argsort()[:-num_top_words - 1:-1]]
        top_word.append(topic_top_word)
    return top_word

def run_lda_model(texts: list, num_topics: int, num_top_words: int, num_iters: int,
                  max_df: float, min_df: float):
    """
    Given a training corpus, return a list of the topic words by LDA
    :param num_topics: number of topics
    :param num_top_words: the number of words in a topic
    :param num_iters: the maximum number of iterations
    :param max_df: when building the vocabulary ignore terms that have a document frequency strictly higher than the given threshold (corpus-specific stop words).
                   If float, the parameter represents a proportion of documents, integer absolute counts
    :param min_df: when building the vocabulary ignore terms that have a document frequency strictly lower than the given threshold.
                   This value is also called cut-off in the literature. If float, the parameter represents a proportion of documents, integer absolute counts
    :return: the list of top topic words
    """
    tf_vectorizer = CountVectorizer(max_df=max_df, min_df=min_df, max_features=None)
    corpus_tf = tf_vectorizer.fit_transform(texts)
    lda = LatentDirichletAllocation(n_components=num_topics, max_iter=num_iters,
                                    learning_method='batch')
    lda.fit(corpus_tf)
    tf_feature_names = tf_vectorizer.get_feature_names()
    top_word_list = sklearn_model_get_top_words(lda, tf_feature_names, num_top_words)
    return top_word_list

def run_nmf_model(texts: list, num_topics: int, num_top_words: int, num_iters: int,
                  max_df: float, min_df: float):
    """
    Given a training corpus, return a list of the topic words by NMF
    :param num_topics: number of topics
    :param num_top_words: the number of words in a topic
    :param num_iters: the maximum number of iterations
    :param max_df: When building the vocabulary ignore terms that have a document frequency strictly higher than the given threshold (corpus-specific stop words).
                   If float, the parameter represents a proportion of documents, integer absolute counts.
    :param min_df: When building the vocabulary ignore terms that have a document frequency strictly lower than the given threshold.
                   This value is also called cut-off in the literature. If float, the parameter represents a proportion of documents, integer absolute counts.
    :return: the list of top topic words
    """
    tfidf_vectorizer = TfidfVectorizer(max_df=max_df, min_df=min_df, max_features=None)
    corpus_tfidf = tfidf_vectorizer.fit_transform(texts)
    nmf = NMF(n_components=num_topics, random_state=1,
              beta_loss='kullback-leibler', solver='mu', max_iter=num_iters, alpha=0.1,
              l1_ratio=0.5)
    nmf.fit(corpus_tfidf)
    tfidf_feature_names = tfidf_vectorizer.get_feature_names()
    top_word_list = sklearn_model_get_top_words(nmf, tfidf_feature_names, num_top_words)
    return top_word_list


def cluster_texts(texts, clusters=10):
    """ Transform texts to Tf-Idf coordinates and cluster texts using K-Means """
    tfidf_vectorizer = TfidfVectorizer(max_df=0.95, min_df=2, lowercase=True)
    tfidf_model = tfidf_vectorizer.fit_transform(texts)
    km_model = KMeans(n_clusters=clusters)
    km_model.fit(tfidf_model)

    clustering = collections.defaultdict(list)

    for idx, label in enumerate(km_model.labels_):
        clustering[label].append(idx)

    print("Top terms per cluster:")
    order_centroids = km_model.cluster_centers_.argsort()[:, ::-1]
    terms = tfidf_vectorizer.get_feature_names()
    for i in range(clusters):
        print("Cluster %d:" % i)
        for ind in order_centroids[i, :10]:
            print(' %s' % terms[ind])
    return clustering

if __name__ == '__main__':
    path = "imsdb_scenes_dialogs_nov_2015/scenes"
    genres = os.listdir(path)

    fout = open("topic_words.txt", "w")
    fout.write("Genre"+","+"Topic Words"+"\n")
    for genre in genres:
        files = [file for file in os.listdir(path + "/" + genre) if file.endswith(".txt")]
        all_texts = []
        for file in files:
            with open(path + "/" + genre + "/" + file) as fin:
                text = fin.read()
            all_texts.append(text)
        while len(all_texts) < 10:
            all_texts = flatten_list([[text[:len(text)//2], text[len(text)//2:]] for text in all_texts])
        corpus = nlp_preprocessing(all_texts, stemming=False)
        nmf_result = run_nmf_model(corpus, 5, 2, 1000, 0.85, 0.15)
        fout.write(genre + "," + ",".join(flatten_list(nmf_result))+"\n")
    fout.close()
