class RNNModel:
    def __init__(self, vocab_size, embedding_dim, rnn_units):
        self.vocab_size = vocab_size
        self.embedding_dim = embedding_dim
        self.rnn_units = rnn_units
        self.model = self.build_model()

    def build_model(self):
        from tensorflow.keras.models import Sequential
        from tensorflow.keras.layers import Embedding, SimpleRNN, Dense

        model = Sequential()
        model.add(Embedding(input_dim=self.vocab_size, output_dim=self.embedding_dim))
        model.add(SimpleRNN(self.rnn_units, return_sequences=True))
        model.add(Dense(self.vocab_size, activation='softmax'))
        return model

    def compile_model(self, learning_rate=0.001):
        from tensorflow.keras.optimizers import Adam

        self.model.compile(loss='sparse_categorical_crossentropy', optimizer=Adam(learning_rate), metrics=['accuracy'])

    def summary(self):
        return self.model.summary()