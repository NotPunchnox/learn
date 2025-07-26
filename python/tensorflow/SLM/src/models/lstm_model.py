from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import LSTM, Dense, Embedding, Dropout
from tensorflow.keras.optimizers import Adam

class LSTMModel:
    def __init__(self, vocab_size, embedding_dim, lstm_units, max_length):
        self.vocab_size = vocab_size
        self.embedding_dim = embedding_dim
        self.lstm_units = lstm_units
        self.max_length = max_length
        self.model = self.build_model()

    def build_model(self):
        model = Sequential()
        model.add(Embedding(input_dim=self.vocab_size, output_dim=self.embedding_dim, input_length=self.max_length))
        model.add(LSTM(self.lstm_units, return_sequences=True))
        model.add(Dropout(0.2))
        model.add(LSTM(self.lstm_units))
        model.add(Dropout(0.2))
        model.add(Dense(self.vocab_size, activation='softmax'))
        return model

    def compile_model(self, learning_rate=0.001):
        self.model.compile(loss='sparse_categorical_crossentropy', optimizer=Adam(learning_rate=learning_rate), metrics=['accuracy'])

    def summary(self):
        return self.model.summary()