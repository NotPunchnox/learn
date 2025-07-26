def load_data(file_path):
    with open(file_path, 'r', encoding='utf-8') as file:
        lines = file.readlines()
    questions, answers = [], []
    for line in lines:
        question, answer = line.strip().split('\t')
        questions.append(question)
        answers.append(answer)
    return questions, answers

def clean_text(text):
    text = text.lower()
    text = ''.join(char for char in text if char.isalnum() or char.isspace())
    return text.strip()

def prepare_data(questions, answers):
    questions = [clean_text(q) for q in questions]
    answers = [clean_text(a) for a in answers]
    return questions, answers