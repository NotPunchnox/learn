class Tokenizer:
    def __init__(self, texts):
        self.texts = texts
        self.word_index = {}
        self.index_word = {}
        self.vocab_size = 0
        self.create_vocab()

    def create_vocab(self):
        unique_words = set()
        for text in self.texts:
            for word in text.split():
                unique_words.add(word)
        
        self.vocab_size = len(unique_words)
        self.word_index = {word: idx + 1 for idx, word in enumerate(unique_words)}
        self.index_word = {idx + 1: word for idx, word in enumerate(unique_words)}

    def texts_to_sequences(self, texts):
        sequences = []
        for text in texts:
            seq = [self.word_index.get(word, 0) for word in text.split()]
            sequences.append(seq)
        return sequences

    def sequences_to_texts(self, sequences):
        texts = []
        for seq in sequences:
            text = ' '.join([self.index_word.get(idx, '') for idx in seq])
            texts.append(text)
        return texts

    def fit_on_texts(self, texts):
        self.texts = texts
        self.create_vocab()