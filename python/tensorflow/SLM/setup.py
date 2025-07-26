from setuptools import setup, find_packages

setup(
    name='neural-language-model',
    version='0.1.0',
    author='Your Name',
    author_email='your.email@example.com',
    description='A simple language model using RNN and LSTM for question answering.',
    packages=find_packages(where='src'),
    package_dir={'': 'src'},
    install_requires=[
        'tensorflow>=2.0.0',
        'keras>=2.0.0',
        'numpy',
        'pandas',
        'scikit-learn',
        'jupyter'
    ],
    classifiers=[
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',
        'Operating System :: OS Independent',
    ],
    python_requires='>=3.6',
)