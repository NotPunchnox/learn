def save_model(model, filepath):
    model.save(filepath)

def load_model(filepath):
    from keras.models import load_model
    return load_model(filepath)

def plot_training_history(history):
    import matplotlib.pyplot as plt

    plt.plot(history.history['loss'], label='loss')
    plt.plot(history.history['val_loss'], label='val_loss')
    plt.title('Model Loss')
    plt.ylabel('Loss')
    plt.xlabel('Epoch')
    plt.legend()
    plt.show()