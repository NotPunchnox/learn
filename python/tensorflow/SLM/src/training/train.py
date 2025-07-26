# FILE: /neural-language-model/neural-language-model/src/training/train.py

import numpy as np
from keras.models import Sequential
from keras.layers import Embedding, SimpleRNN, Dense
from keras.preprocessing.sequence import pad_sequences
from src.data.preprocessing import load_data, prepare_data
from src.models.rnn_model import RNNModel

def train_model(epochs=100, batch_size=32):
    # Load and preprocess data
    questions, answers = load_data('data/sample_qa.txt')
    X, y, tokenizer, max_length = prepare_data(questions, answers)

    # Pad sequences
    X = pad_sequences(X, maxlen=max_length, padding='post')
    y = np.expand_dims(y, -1)

    # Initialize and compile the model
    model = RNNModel(vocab_size=len(tokenizer.word_index) + 1, max_length=max_length)
    model.compile(optimizer='adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])

    # Train the model
    model.fit(X, y, epochs=epochs, batch_size=batch_size, validation_split=0.2)

if __name__ == "__main__":
    train_model()