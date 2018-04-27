import numpy as np
from sklearn import svm
from sklearn.model_selection import train_test_split
from sklearn import preprocessing

def iris_type(s):
	it = {'Action': 0, 'Adventure': 1, 'Animation': 2, 'Biography': 3, 'Comedy': 4, 'Crime': 5, 
	'Drama': 6, 'Family': 7, 'Fantasy': 8, 'Film-Noir':9, 'History': 10, 'Horror': 11, 'Music': 12,
	'Musical': 13, 'Mystery': 14, 'Romance': 15, 'Sci-Fi': 16, 'Short': 17, 'Sport': 18, 'Thriller': 19,
	'War': 20, 'Western': 21}
	return it[s]

path = '../classify_input.txt' 
data = np.loadtxt(path, dtype=str, delimiter=' ', converters={1: iris_type})

x, y = np.split(data, (1,), axis=1)
x = x[:, :1]
le = preprocessing.LabelEncoder()
x = le.fit_transform(x)

x_train, x_test, y_train, y_test = train_test_split(x,y,random_state=0,train_size=0.8)

#print(x)

#x_train, x_test, y_train, y_test = train_test_split(x, y, random_state=1, train_size=6)

clf = svm.SVC(C=1.0, kernel='rbf', gamma=20, decision_function_shape='ovr')

clf.fit(x_train.reshape(-1,1), y_train.reshape(-1,1))

print clf.score(x_train.reshape(-1,1), y_train.reshape(-1,1)) # whether the training data is good enough
print clf.score(x_test.reshape(-1,1), y_test.reshape(-1,1)) # validation

testpath = path = '../models/processeduser/AutoPhrase_single-word.txt' 
testdata = np.loadtxt(path, dtype=str, delimiter='	')
testing_y, testing_x = np.split(testdata, (1,), axis=1)
testing_top, testing_down = np.split(testing_x, (20,), axis=0) # extract the top 20 quality words

output = clf.predict(le.fit_transform(testing_top).reshape(-1,1))
print clf.predict(le.fit_transform(testing_top).reshape(-1,1))

fl = open("../classify_tag.txt",'w')
for item in output:
  fl.write("%s\n" % item)
fl.close()