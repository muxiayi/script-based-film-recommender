import math
import sys
import time
import metapy
import pytoml

class PL2Ranker(metapy.index.RankingFunction):
    """
    Create a new ranking function in Python that can be used in MeTA.
    """
    def __init__(self, some_param=1.0):
        self.param = some_param
        # You *must* call the base class constructor here!
        super(PL2Ranker, self).__init__()

    def score_one(self, sd):
        tfn = sd.doc_term_count*math.log(1.0+self.param*sd.avg_dl/float(sd.doc_size),2)
        lamb = float(sd.num_docs)/float(sd.corpus_term_count)
        frac = (tfn*1.0*math.log(tfn*lamb,2)+(math.log(math.e,2))*(1.0/lamb-tfn)+0.5*math.log(2*math.pi*tfn,2))/float(tfn+1.0)
        if lamb<1.0 or tfn<=0:
            return 0
        return sd.query_term_weight*frac*1.0

def load_ranker(cfg_file):
    #return metapy.index.OkapiBM25(k1=1.2,b=0.75,k3=500)
    return PL2Ranker(3.0)
    #return metapy.index.DirichletPrior(mu=230.0)

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Usage: {} config.toml".format(sys.argv[0]))
        sys.exit(1)

    cfg = sys.argv[1]
    print('Building or loading index...')
    #f1=open('./search_engine/config.toml','r')
    #print f1.read() #open success
    idx = metapy.index.make_inverted_index(cfg)
    ranker = load_ranker(cfg)
    ev = metapy.index.IREval(cfg)
    
    with open(cfg, 'r') as fin:
        cfg_d = pytoml.load(fin)

    query_cfg = cfg_d['query-runner']
    if query_cfg is None:
        print("query-runner table needed in {}".format(cfg))
        sys.exit(1)

    start_time = time.time()
    top_k = 5
    query_path = query_cfg.get('query-path', 'queries.txt')
    query_start = query_cfg.get('query-id-start', 0)

    query = metapy.index.Document()
    #print('Running queries')
    f=open('../relevance.txt','w')
    with open(query_path) as query_file:
        for query_num, line in enumerate(query_file):
            query.content(line.strip())
            results = ranker.score(idx, query, top_k)
            #avg_p = ev.avg_p(results, query_start + query_num, top_k)
            print>>f,results
            #print("Query {} average precision: {}".format(query_num + 1, avg_p))
    #ev.map()
    f.close()
    #print("Mean average precision: {}".format(ev.map()))
    #print("Elapsed: {} seconds".format(round(time.time() - start_time, 4)))
