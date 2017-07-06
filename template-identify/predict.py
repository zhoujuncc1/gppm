import argparse 
import tensorflow as tf
import sys, os
import math
def load_graph(frozen_graph_filename):
    # We load the protobuf file from the disk and parse it to retrieve the 
    # unserialized graph_def
    with tf.gfile.GFile(frozen_graph_filename, "rb") as f:
        graph_def = tf.GraphDef()
        graph_def.ParseFromString(f.read())

    # Then, we can use again a convenient built-in function to import a graph_def into the 
    # current default Graph
    with tf.Graph().as_default() as graph:
        tf.import_graph_def(
            graph_def, 
            input_map=None, 
            return_elements=None, 
            name="prefix", 
            op_dict=None, 
            producer_op_list=None
        )
    return graph

def predict(x_value):
    # Let's allow the user to pass the filename as an argument
    parser = argparse.ArgumentParser()
    parser.add_argument("--frozen_model_filename", default="checkpoints/frozen_model.pb", type=str, help="Frozen model file to import")
    args = parser.parse_args()

    # We use our "load_graph" function
    graph = load_graph(args.frozen_model_filename)

    # We can verify that we can access the list of operations in the graph
    #for op in graph.get_operations():
        #print(op.name)
        # prefix/Placeholder/inputs_placeholder
        # ...
        # prefix/Accuracy/predictions
        
    # We access the input and output nodes 
    x = graph.get_tensor_by_name('prefix/input:0')
    
    y_conv = graph.get_tensor_by_name('prefix/y_conv:0')
    out  = tf.sigmoid(y_conv)
    keep_prob = graph.get_tensor_by_name('prefix/keep_prob:0') 
    # We launch a Session
    with tf.Session(graph=graph) as sess:
        # Note: we didn't initialize/restore anything, everything is stored in the graph_def
        y_out = sess.run(out, feed_dict={
            x: x_value,
            keep_prob: 1.0
        })
        print(y_out)
def runModel(model, specie, size):
    for i in range(0, size):
        name = model+'_x'+str(specie)+'_'+str(i)+'.txt'
        print(name)
        file = open('paper_test/'+name, 'r')
        x_value = eval(file.read())
        file.close()
        padding = 200-len(x_value)
        for _ in range(0, padding):
            x_value.append(x_value[-1])
        predict(x_value)

def run0():    
    for name in os.listdir('paper_test'):
        if '_0.txt' in name:
            print(name)
            file = open('paper_test/'+name, 'r')
            x_value = eval(file.read())
            file.close()
            padding = 200-len(x_value)
            for _ in range(0, padding):
                x_value.append(x_value[-1])
            predict(x_value)

if __name__ == '__main__':
    runModel('mapk', 7, 20)

