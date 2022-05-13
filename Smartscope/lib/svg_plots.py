import drawSvg as draw
from drawSvg import elements as elementsModule
from math import floor, sqrt
from io import StringIO

from matplotlib.pyplot import legend
from Smartscope.lib.config import *
import logging

mainlog = logging.getLogger('mainlog')

# class Svg:
#     content = ''

#     def load(self, file):
#         with open(file, 'r') as f:
#             svg = f.readlines()
#         self.header = svg[0]
#         self.png = svg[1]
#         self.content = svg[2:-1]

#     def set_size(self, x, y):
#         self.sizeX = x
#         self.sizeY = y
#         self.header = f"""<svg viewBox="0 0 {y} {x}" xmlns="http://www.w3.org/2000/svg" xmlns:xlink= "http://www.w3.org/1999/xlink">\n"""

#     def add_bg_image(self, path, id=''):
#         if id != '':
#             id = f'id="{id}" '
#         img = f"""<image {id}xlink:href="{path}" x="0" y="0" height="{self.sizeX}" width="{self.sizeY}" />\n"""
#         self.png = img

#     def add_rect(self, x, y, width, height, rotation=None, id=''):
#         if id != '':
#             id = f'id="{id}" '
#         rect = f"""<rect {id}x="{x}" y="{y}" width="{width}" height="{height}" fill="green" fill-opacity="0" stroke-width="5" stroke="green" />\n"""
#         if rotation is not None:
#             transform = f"""<g transform = "rotate({rotation} {x+width//2} {y+height//2})">/n{rect}</g>\n"""
#             return transform
#         else:
#             return rect

#     def add_circ(self, x, y, radius, id=''):
#         if id != '':
#             id = f'id="{id}" '
#         circ = f"""<circle {id}cx="{x}" cy="{y}" r="{radius}" fill="green" fill-opacity="0" stroke-width="5" stroke="green" />"""
#         return circ

#     def add_anchor(self, func, *, link=''):
#         def inner(*args, **kwargs):
#             inside = func(*args, **kwargs)
#             outside = f"""<a xlink:href="{link}">\n{inside}</a>\n"""
#             return outside
#         return inner

#     def save(self, file):
#         with open(file, 'w') as f:
#             f.write(self.header)
#             f.write(self.png)
#             f.write(self.content)
#
#
#         f.write('</svg>')
# DEFAULT_COLORS = ['blue', 'purple', 'red', 'gray', 'white']


def add_scale_bar(pixelsize, w, h, id_type='atlas'):
    ft_sz = floor(w / 40)
    scalebarGroup = draw.Group(id='scaleBar_' + id_type)
    startpoint = w * 0.98
    if pixelsize > 500:
        text = '100 \u03BCm'
        lineLenght = 1000000 / pixelsize
    elif pixelsize > 10:
        text = '10 \u03BCm'
        lineLenght = 100000 / pixelsize
    line = draw.Line(startpoint - lineLenght, h * 0.02, startpoint, h * 0.02, stroke='white', stroke_width=ft_sz / 2, id=f'line_{id_type}')
    text = draw.Text(text, ft_sz, path=line, fill='white', text_anchor='middle', lineOffset=-0.5, id=f'text_{id_type}')
    scalebarGroup.append(line)
    scalebarGroup.append(text)
    return scalebarGroup


def add_legend(label_list, w, h, prefix):
    startpoint = h * 0.96
    ft_sz = h * 0.03
    step = h * 0.035
    legend = draw.Group(id='legend')
    box = draw.Rectangle(w * 0.01, startpoint - (step * (len(label_list) + 0.25)), w * 0.25, step * (len(label_list) + 1.25),
                         fill='gray', stroke='black', stroke_width=floor(ft_sz / 5), opacity=0.6)
    legend.append(box)
    t = draw.Text(f"Legend", ft_sz, x=w * 0.02, y=startpoint, paint_order='stroke',
                  stroke_width=floor(ft_sz / 5), stroke='black', fill="white")
    legend.append(t)
    for (color, label, prefix) in label_list:
        startpoint -= step
        t = draw.Text(f"{prefix} {label}", ft_sz, x=w * 0.02, y=startpoint, paint_order='stroke',
                      stroke_width=floor(ft_sz / 5), stroke='black', class_='legend', label=label, fill=color)
        mainlog.debug(t.__dict__)

        legend.append(t)
    return legend


class myDrawging(draw.Drawing):
    def asSvg(self, outputFile=None):
        returnString = outputFile is None
        if returnString:
            outputFile = StringIO()
        startStr = '''<?xml version="1.0" encoding="UTF-8"?>
<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink"
      viewBox="{} {} {} {}"'''.format(
            *self.viewBox)
        endStr = '</svg>'
        outputFile.write(startStr)
        elementsModule.writeXmlNodeArgs(self.svgArgs, outputFile)
        outputFile.write('>\n<defs>\n')
        # Write definition elements

        def idGen(base=''):
            idStr = self.idPrefix + base + str(self.idIndex)
            self.idIndex += 1
            return idStr
        prevSet = set((id(defn) for defn in self.otherDefs))

        def isDuplicate(obj):
            nonlocal prevSet
            dup = id(obj) in prevSet
            prevSet.add(id(obj))
            return dup
        for element in self.otherDefs:
            try:
                element.writeSvgElement(idGen, isDuplicate, outputFile, False)
                outputFile.write('\n')
            except AttributeError:
                pass
        allElements = self.allElements()
        for element in allElements:
            try:
                element.writeSvgDefs(idGen, isDuplicate, outputFile, False)
            except AttributeError:
                pass
        outputFile.write('</defs>\n')
        # Generate ids for normal elements
        prevDefSet = set(prevSet)
        for element in allElements:
            try:
                element.writeSvgElement(idGen, isDuplicate, outputFile, True)
            except AttributeError:
                pass
        prevSet = prevDefSet
        # Write normal elements
        for element in allElements:
            try:
                element.writeSvgElement(idGen, isDuplicate, outputFile, False)
                outputFile.write('\n')
            except AttributeError:
                pass
        outputFile.write(endStr)
        if returnString:
            return outputFile.getvalue()


def drawAtlas(atlas, targets, display_type, method):
    plugins = load_plugins()

    # d = myDrawging(atlas.shape_y // atlas.binning_factor, atlas.shape_x // atlas.binning_factor, id='square-svg', displayInline=False)
    d = myDrawging(atlas.shape_y, atlas.shape_x, id='square-svg', displayInline=False)
    # print(atlas.png)
    d.append(draw.Image(0, 0, d.width, d.height, path=atlas.png['url'], embed=False))
    shapes = draw.Group(id='atlasShapes')
    text = draw.Group(id='atlasText')

    labels_list = []
    for i in targets:
        color, label, prefix = i.css_color(plugins, display_type, method)
        if color is not None:  # style = f"stroke: {color}; fill: {color}; color: {color}; "
            sz = floor(sqrt(i.area))
            x = i.finders[0].x - sz // 2
            y = -(i.finders[0].y - sz // 2) + d.height - sz
            r = draw.Rectangle(x, y, sz, sz, id=i.pk, stroke_width=floor(d.width / 300), stroke=color, fill=color, fill_opacity=0, label=label,
                               class_=f'target', onclick="clickSquare(this)")  # style=style,

            if i.selected:
                ft_sz = floor(d.width / 35)
                t = draw.Text(str(i.number), ft_sz, x=x + sz, y=y + sz, id=f'{i.pk}_text', paint_order='stroke',
                              stroke_width=floor(ft_sz / 5), stroke=color, fill='white', class_=f'svgtext {i.status}')
                text.append(t)
                r.args['class'] += f" {i.status}"
                if i.status == 'completed':
                    if i.has_active:
                        r.args['class'] += ' has_active'
                    elif i.has_queued:
                        r.args['class'] += ' has_queued'
                    elif i.has_completed:
                        r.args['class'] += ' has_completed'
            labels_list.append((color, label, prefix))
            shapes.append(r)
    d.append(shapes)
    d.append(text)
    # d.append(add_scale_bar(atlas.pixel_size * atlas.binning_factor, d.width, d.height))
    d.append(add_scale_bar(atlas.pixel_size, d.width, d.height))
    d.append(add_legend(set(labels_list), d.width, d.height, atlas.pixel_size))
    # d.setRenderSize()
    # print(d.asSvg())
    return d.asSvg()


def drawSquare(square, targets, display_type, method):
    plugins = load_plugins()
    d = myDrawging(square.shape_y, square.shape_x, id='square-svg', displayInline=False)
    # d = myDrawging(square.shape_y // square.binning_factor, square.shape_x // square.binning_factor, displayInline=False)
    # print(square.png)
    d.append(draw.Image(0, 0, d.width, d.height, path=square.png['url'], embed=False))
    shapes = draw.Group(id='squareShapes')
    text = draw.Group(id='squareText')
    labels_list = []
    bis_groups = {}
    for i in targets:
        color, label, prefix = i.css_color(plugins, display_type, method)
        if color is not None:
            x = i.finders[0].x
            y = -(i.finders[0].y) + d.height
            # qualityClass = f'quality-{i.quality}' if i.quality is not None else ''
            c = draw.Circle(x, y, i.radius, id=i.pk, stroke_width=floor(d.width / 250), stroke=color, fill=color, fill_opacity=0, label=label,
                            class_=f'target', number=i.number, onclick="clickHole(this)")

            if i.selected:
                ft_sz = floor(d.width / 3000 * 80)
                t = draw.Text(str(i.number), ft_sz, x=x + i.radius, y=y + i.radius, id=f'{i.pk}_text', paint_order='stroke',
                              stroke_width=floor(ft_sz / 5), stroke=color, fill='white', class_=f'svgtext {i.status}')  # + qualityClass
                text.append(t)
            if i.status is not None:
                c.args['class'] += f" {i.status}"
                c.args['fill-opacity'] = 0.6 if color != 'blue' else 0
            if i.bis_type is not None:
                c.args['class'] += f" {i.bis_type}"
                if i.bis_type == 'center':
                    c.args['stroke-width'] = floor(d.width / 200)
            if i.bis_group in bis_groups.keys():
                bis_groups[i.bis_group].append(c)
            else:
                bis_groups[i.bis_group] = [c]
            labels_list.append((color, label, prefix))
    for bis_group, item in bis_groups.items():
        g = draw.Group(id=bis_group)
        for i in item:
            g.append(i)
        shapes.append(g)
    d.append(shapes)
    d.append(text)
    # d.append(add_scale_bar(square.pixel_size * square.binning_factor, d.width, d.height, id_type='square'))
    d.append(add_scale_bar(square.pixel_size, d.width, d.height, id_type='square'))
    d.append(add_legend(set(labels_list), d.width, d.height, square.pixel_size))
    # d.setRenderSize()
    # print(d.asSvg())
    return d.asSvg()


if __name__ == "__main__":
    s = Svg()
    s.set_size(400, 400)
    s.add_bg_image("test_db/1_grid1/pngs/grid1_square0_hole65.png")
    r = s.add_anchor(s.add_rect, link='http://www.google.com')
    s.content += r(100, 100, 10, 10, rotation=45)

    s.save("/Users/bouvettej2/Documents/Cryolo/test.svg")
