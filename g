import os
import subprocess

input_dir = 'input'
output_dir = 'output'
generated_output_dir = 'generated_output'

if not os.path.exists(generated_output_dir):
    os.makedirs(generated_output_dir)

# Iterate over input files
for input_file in os.listdir(input_dir):
    input_path = os.path.join(input_dir, input_file)
    output_file = f'generated_{input_file}'
    output_path = os.path.join(generated_output_dir, output_file)
    
    # Run your program with the input file and save the output
    with open(input_path, 'r') as infile, open(output_path, 'w') as outfile:
        subprocess.run(['your_program.exe'], stdin=infile, stdout=outfile)
    
    # Compare the generated output with the expected output
    expected_output_path = os.path.join(output_dir, f'expected_{input_file}')
    with open(output_path, 'r') as generated_file, open(expected_output_path, 'r') as expected_file:
        generated_output = generated_file.read()
        expected_output = expected_file.read()
        
        if generated_output == expected_output:
            print(f'Test {input_file} passed.')
        else:
            print(f'Test {input_file} failed.')