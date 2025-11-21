#!/usr/bin/env python3
import re
import subprocess
from graphviz import Digraph

class CppClassParser:
    def __init__(self):
        self.classes = {}
        self.inheritance = []
    
    def parse_file(self, filename):
        with open(filename, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
            
        # 解析类定义
        class_pattern = r'class\s+(\w+)\s*(:\s*public\s+(\w+))?'
        classes = re.findall(class_pattern, content)
        
        for class_name, _, base_class in classes:
            self.classes[class_name] = {
                'methods': self.extract_methods(content, class_name),
                'base': base_class if base_class else None
            }
            if base_class:
                self.inheritance.append((class_name, base_class))
    
    def extract_methods(self, content, class_name):
        # 简化版方法提取
        method_pattern = rf'{class_name}::(\w+)\s*\('
        return re.findall(method_pattern, content)
    
    def generate_diagram(self):
        dot = Digraph(comment='MFC Framework Architecture')
        
        # 添加类节点
        for class_name, info in self.classes.items():
            label = f"{class_name}|"
            for method in info['methods'][:5]:  # 只显示前5个方法
                label += f"{method}()\\l"
            dot.node(class_name, f"{{{label}}}", shape='record')
        
        # 添加继承关系
        for child, parent in self.inheritance:
            if parent in self.classes:
                dot.edge(parent, child, arrowhead='empty')
        
        dot.render('mfc_architecture', format='png', cleanup=True)
        return dot

# 使用示例
if __name__ == "__main__":
    parser = CppClassParser()
    files = ["_afx.h", "_AFXWIN.h", "_AFXTLS.h", "_AFXSTAT.h"]
    for file in files:
        try:
            parser.parse_file(file)
        except FileNotFoundError:
            print(f"Warning: {file} not found")
    
    diagram = parser.generate_diagram()
    print("架构图已生成: mfc_architecture.png")