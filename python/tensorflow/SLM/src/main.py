import numpy as np
import pandas as pd
from keras.models import Sequential
from keras.layers import Embedding, SimpleRNN, Dense
from keras.preprocessing.sequence import pad_sequences
from keras_hub.tokenizers import Tokenizer
from sklearn.model_selection import train_test_split

# Load and preprocess data
def load_data(filepath):
    data = pd.read_csv(filepath, sep='\t', header=None, names=['question', 'answer'])
    return data

def preprocess_data(data):
    questions = data['question'].values
    answers = data['answer'].values
    return questions, answers

# Build RNN model
def build_rnn_model(vocab_size, embedding_dim, input_length):
    model = Sequential()
    model.add(Embedding(input_dim=vocab_size, output_dim=embedding_dim, input_length=input_length))
    model.add(SimpleRNN(128, return_sequences=False))
    model.add(Dense(vocab_size, activation='softmax'))
    model.compile(loss='sparse_categorical_crossentropy', optimizer='adam', metrics=['accuracy'])
    return model

# Main function
if __name__ == "__main__":
    # Load data
    data = load_data('data/sample_qa.txt')
    questions, answers = preprocess_data(data)

    print("Questions:", questions[:5])
    print("Answers:", answers[:5])

    # Tokenization
    tokenizer = Tokenizer()
    tokenizer.fit_on_texts(questions)
    vocab_size = len(tokenizer.word_index) + 1
    max_length = max([len(q.split()) for q in questions])

    # Prepare input-output pairs
    question_sequences = tokenizer.texts_to_sequences(questions)
    answer_sequences = tokenizer.texts_to_sequences(answers)
    question_padded = pad_sequences(question_sequences, maxlen=max_length, padding='post')
    answer_padded = pad_sequences(answer_sequences, maxlen=max_length, padding='post')

    # Train-test split
    X_train, X_test, y_train, y_test = train_test_split(question_padded, answer_padded, test_size=0.2)

    # Build and train model
    model = build_rnn_model(vocab_size, embedding_dim=50, input_length=max_length)
    model.fit(X_train, y_train, epochs=100, batch_size=32, validation_split=0.2)

    # Function to generate response
    def generate_response(question):
        seq = tokenizer.texts_to_sequences([question])
        padded = pad_sequences(seq, maxlen=max_length, padding='post')
        pred = model.predict(padded)
        pred_ids = np.argmax(pred, axis=-1)
        response = [tokenizer.index_word.get(idx, '') for idx in pred_ids[0]]
        return ' '.join(response).strip()

    # Test the model
    test_question = "Quel est ton nom ?"
    print(generate_response(test_question))