import seaborn as sns
import matplotlib.pyplot as plt
import optimizer.key_transitions as kt
import pandas as pd

if __name__ == '__main__':
    three_transitions = {
        'ktg_exponential5': 5.0,
        'ktg_exponential10': 10.0,
        'ktg_exponential15': 15.0
    }
    kts = { three_transitions[k]: v for k, v in kt._kt.items() if k in three_transitions }
    df = pd.DataFrame(kts)
    keys = ['C', 'Db', 'D', 'Eb', 'E', 'F', 'F#', 'G', 'Ab', 'A', 'Bb', 'B',
            'c', 'c#', 'd', 'eb', 'e', 'f', 'f#', 'g', 'ab', 'a', 'bb', 'b']
    df['Key'] = keys
    df = df.melt(id_vars=['Key'], var_name='Ratio', value_name='Probability')
    sns.catplot(x='Key', y='Probability', hue='Ratio', data=df, height=3.0, aspect=2.0, legend_out=True, kind='bar')
    plt.show()

